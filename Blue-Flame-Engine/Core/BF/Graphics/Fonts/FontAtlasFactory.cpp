﻿#include "FontAtlasFactory.h"
#include "BF/System/Log.h"
#include "BF/IO/ResourceManager.h"

namespace BF
{
	namespace Graphics
	{
		namespace Fonts
		{
			using namespace std;
			using namespace BF::Graphics::API;
			using namespace BF::Math;
			using namespace BF::IO;

			struct Glyph
			{
				unsigned int unicode;
				unsigned int width, height;
				unsigned int bufferSize;
				uint8_t* buffer;

				Vector2 bearing;
				Vector2 advance;

				Glyph() = default;
				Glyph(unsigned int unicode, unsigned int width, unsigned int height, Vector2 bearing, Vector2 advance, uint8_t* data) :
					unicode(unicode), width(width), height(height), bearing(bearing), advance(advance)
				{
					bufferSize = width * height;
					buffer = new uint8_t[bufferSize];
					memcpy(buffer, data, sizeof(uint8_t) * bufferSize);
				}
			};

			FT_Library FontAtlasFactory::library;
			FT_Error FontAtlasFactory::error;
			FT_Face FontAtlasFactory::face;

			bool FontAtlasFactory::freeTypeInitialized = false;

			unsigned int FontAtlasFactory::startUnicode = 0;
			unsigned int FontAtlasFactory::endUnicode = 0;
			
			FontAtlas* FontAtlasFactory::GetFontAtlas(const string& filename, unsigned int charPixelSize, Language language, const Shader& shader)
			{
				if (ResourceManager::Exist(filename + to_string(charPixelSize)))
				{
					return (FontAtlas*)ResourceManager::GetResource(filename + to_string(charPixelSize));
				}
				else
				{
					LoadFont(filename);

					ResourceManager::Add(filename + to_string(charPixelSize), Create(face->family_name, charPixelSize, language, shader));
					return (FontAtlas*)ResourceManager::GetResource(filename + to_string(charPixelSize));
				}
			}

			void FontAtlasFactory::LoadFont(const string& filename)
			{
				if (!freeTypeInitialized)
				{
					error = FT_Init_FreeType(&library);
					if (error != FT_Err_Ok)
						BF_LOG_ERROR("Could not initialize FreeType !");
					else
						freeTypeInitialized = true;
				}

				error = FT_New_Face(library, filename.c_str(), 0, &face);
				if (error == FT_Err_Unknown_File_Format)
					BF_LOG_ERROR("FreeType: Unknown file format !");
				else if (error)
					BF_LOG_ERROR("FreeType: file not found !");

				//cout << "family_name: " << face->family_name << " style_name: " << face->style_name << endl;
				//cout << "face_index: " << face->face_index << " num_faces: " << face->num_faces << " num_glyphs: " << face->num_glyphs << endl;
			}

			FontAtlas* FontAtlasFactory::Create(const string& fontName, unsigned int charPixelSize, Language language, const Shader& shader)
			{
				vector<Character>* characters = new vector<Character>();

				error = FT_Set_Pixel_Sizes(face, 0, charPixelSize);
				if (error != FT_Err_Ok)
					BF_LOG_ERROR("FreeType: could not set pixel size !");

				if (language == Language::English)
				{
					startUnicode = 0x00000020;
					endUnicode = 0x0000007E;
				}

				const unsigned int totalGlyphs = endUnicode - startUnicode;

				vector<Glyph> glyph(totalGlyphs);
				unsigned int totalGlyphsWidth = 0;

				float maxYBearing = 0.0f;

				for (unsigned int i = 0; i < totalGlyphs; i++)
				{
					unsigned int unicode = startUnicode + i;
					PrepareGlyph(unicode);

					if (face->glyph->bitmap.width == 0)
						face->glyph->bitmap.width = face->glyph->advance.x >> 6;

					if (face->glyph->metrics.horiBearingY > maxYBearing)
						maxYBearing = (float)(face->glyph->metrics.horiBearingY >> 6);

					glyph[i] = Glyph(unicode,
						face->glyph->bitmap.width, face->glyph->bitmap.rows,
						Vector2((float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top),
						Vector2((float)(face->glyph->advance.x >> 6), (float)(face->glyph->advance.y >> 6)), face->glyph->bitmap.buffer);

					totalGlyphsWidth += (unsigned int)face->glyph->bitmap.width;
				}

				unsigned int altalWidth, atlasHeight;
				CalculateTextureAtelsSize(totalGlyphs, totalGlyphsWidth, charPixelSize, altalWidth, atlasHeight);

				TextureAtlas textureAtlas;
				textureAtlas.Create(altalWidth, atlasHeight, Texture::Format::R8);

				for (unsigned int x = 0, y = 0, index = 0; index < totalGlyphs; )
				{
					if (x + glyph[index].width >= altalWidth)
					{
						y++;
						x = 0;
					}
					else
					{
						Rectangle scissorRectangle = Rectangle(x, charPixelSize * y, glyph[index].width, glyph[index].height);
						textureAtlas.AddTexture(scissorRectangle, glyph[index].buffer);

						characters->push_back(Character(Vector2(), scissorRectangle, glyph[index].bearing, glyph[index].advance));
						x += (unsigned int)glyph[index].width;
						index++;
					}
				}

				Texture2D* texture = new Texture2D(shader);
				texture->Create(*textureAtlas.GetTextureData(), Texture2D::Format::R8);
				FT_Done_Face(face);
				//FT_Done_FreeType(library);
				return new FontAtlas(fontName, charPixelSize, maxYBearing, texture, characters);
			}

			void FontAtlasFactory::PrepareGlyph(unsigned int unicode)
			{
				FT_UInt glyph_index = FT_Get_Char_Index(face, unicode);

				error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
				if (error != FT_Err_Ok)
					cout << "ERROR !!" << endl;

				error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

				if (error != FT_Err_Ok)
					cout << "ERROR !!" << endl;
			}

			void FontAtlasFactory::CalculateTextureAtelsSize(unsigned int totalGlyphs, unsigned int totalGlyphsWidth, unsigned int glyphHeight, unsigned int& width, unsigned int& height)
			{
				float square = (float)ceil(sqrt(totalGlyphs));
				float glyphWidth = ceil((float)totalGlyphsWidth / totalGlyphs);
				width = (unsigned int)pow(2, ceil(log(square * glyphWidth) / log(2)));
				float columns = floor(width / (float)glyphWidth);
				float rows = ceil(totalGlyphs / columns);
				height = (unsigned int)pow(2, ceil(log(rows * glyphHeight) / log(2)));

				unsigned int temp = Max((int)height, (int)width);
				height = Min((int)height, (int)width);
				width = temp;
			}
		}
	}
}