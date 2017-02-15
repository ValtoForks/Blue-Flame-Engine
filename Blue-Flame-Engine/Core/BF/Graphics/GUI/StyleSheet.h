#pragma once
#include <string>
#include <unordered_map>
#include <tinyxml2.h>
#include "Widget.h"
#include "BF/Graphics/Renderers/Sprite.h"
#include "BF/Math/Math.h"
#include "BF/Common.h"

namespace BF
{
	namespace Graphics
	{
		namespace GUI
		{
			class BF_API StyleSheet
			{
				private:
					std::string styleSheetNode;
					std::unordered_map<std::string, WidgetData> widgetsData;
					API::Texture2D* texture;

				public:
					StyleSheet(const API::Shader& shader);
					~StyleSheet();

					void Load(const std::string& filename);

					inline const WidgetData& GetWidget(const std::string& name) const { return widgetsData.at(name); }

				private:
					Math::Rectangle ReadWidgetData(const tinyxml2::XMLDocument& xmlDocument, const std::string& name, const std::string& type);
					void LoadWidget(const tinyxml2::XMLDocument& xmlDocument, const std::string& widgetName);
			};
		}
	}
}