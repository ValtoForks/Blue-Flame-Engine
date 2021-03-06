#include "_2DScene.h"

namespace _2DScene
{
	using namespace BF;
	using namespace BF::AI;
	using namespace BF::Application;
	using namespace BF::Graphics;
	using namespace BF::Graphics::API;
	using namespace BF::Graphics::Renderers;
	//using namespace BF::Graphics::GUI;
	using namespace BF::Math;
	using namespace BF::System;

	void DoButtonWork();
	
	bool _2DScene::TimeFunction(void* this_pointer)
	{
		_2DScene* self = static_cast<_2DScene*>(this_pointer);
		if (self->timer.GetElapsedTimeInSeconds() > 5)
		{
			self->timer.Reset();
			return true;
		}
		else
			return false;
	}

	_2DScene::_2DScene() :
		button(*this), button2(*this), button3(*this), /*checkbox(*this), panel(*this),*/ font(spriteRenderer.GetShader()) /*, font2(spriteRenderer.GetShader())*/, waypoint(sprite1, waypoints)/*, renderTarget(spriteRenderer.GetShader())*/ //, spriteAnimation(spriteRenderer, spriteRenderer.GetSpriteShader())
	{
		t = new Texture2D(spriteRenderer.GetShader());
		t2 = new Texture2D(spriteRenderer.GetShader());
		t3 = new Texture2D(spriteRenderer.GetShader());
		t4 = new Texture2D(spriteRenderer.GetShader());
	}

	_2DScene::~_2DScene()
	{
	}

	void _2DScene::Initialize()
	{
		Scene::Initialize();
		//Engine::GetContext().EnableVsync(true);

		camera.Initialize(Matrix4::Orthographic(0.0f, Engine::GetWindow().GetClientWidth(), 0.0f, Engine::GetWindow().GetClientHeight(), -1.0f, 1.0f));
		spriteRenderer.Initialize();
		tileMap.Initialize();
	}

	void _2DScene::Load()
	{
		Scene::Load();

		t->Load("Assets/Textures/tilea5.png");
		t2->Load("Assets/Textures/test2.png");
		t3->Load("Assets/Textures/Untitled.png");
		t4->Load("Assets/Textures/silver.png");

		sprite1 = Sprite(t2, Vector2(0.0f, 0.0f), 0, Color(1.0f, 1.0f, 1.0f, 1.0f));
		sprite2 = Sprite(t, Vector2(0.0f, 0.0f), 1, Color(1.0f));
		sprite3 = Sprite(t, Vector2(300.0f, 100.0f), 0, BF::Math::Rectangle(64, 0, 32, 32), Color(1.0f));
		sprite4 = Sprite(t, Vector2(300.0f, 300.0f), 0, BF::Math::Rectangle(96, 0, 32, 32), Color(1.0f));
		sprite5 = Sprite(t, Vector2(100.0f, 300.0f), 0, BF::Math::Rectangle(128, 0, 32, 32), Color(1.0f));

		sprite6 = Sprite(t2, Vector2(800.0f, 0.0f), 0, Color(1.0f));
		sprite7 = Sprite(t4, BF::Math::Rectangle(0, 0, 1920, 1080), 0, Color(1.0f));
		sprite8 = Sprite(t3, BF::Math::Rectangle(0, 0, 1920, 1080), 0, Color(1.0f));

		font.Load("Assets/Fonts/arial.ttf", 120, BF::Graphics::Fonts::FontAtlasFactory::Language::English);
		text = Text(&font, "Blue Flame Engine !", Vector2(10, 10), 0, Color(0.0f, 0.0f, 1.0f, 1.0f));
		BF_LOG_INFO("%f, %f", text.GetPosition().x, text.GetPosition().y);
		BF_LOG_INFO("%i, %i, %i, %i", text.GetRectangle().x, text.GetRectangle().y, text.GetRectangle().width, text.GetRectangle().height);
		node = Waypoint::Node(Vector2(100.0f, 100.0f));
		node2 = Waypoint::Node(Vector2(300.0f, 100.0f));

		line = LineShape(Vector2(0.0f, 100.0f), Vector2(800.0f, 100.0f), 5, 0, Color(0.0f, 0.0f, 1.0f, 1.0f));
		line2 = LineShape(Vector2(0.0f, 300.0f), Vector2(800.0f, 300.0f), 5, 3, Color(1.0f, 0.0f, 0.0f, 1.0f));

		rp = RegularPolygon(Math::Rectangle(225, 200, 64, 64), 0, Color(0.0f, 0.0f, 1.0f, 1.0f));
		rp2 = RegularPolygon(Math::Rectangle(225, 400, 64, 64), 0, Color(1.0f, 0.0f, 0.0f, 1.0f));
		rpp = RegularPolygon(Math::Rectangle(225, 0, 64, 64), 6, Color(0.0f, 1.0f, 0.0f, 1.0f));

		//waypoints.push_back(Waypoint::Node(Vector2(100.0f, 100.0f), TimeFunction));
		//waypoints.push_back(Waypoint::Node(Vector2(300.0f, 100.0f), TimeFunction));
		//waypoints.push_back(Waypoint::Node(Vector2(300.0f, 300.0f), TimeFunction));
		//waypoints.push_back(Waypoint::Node(Vector2(100.0f, 300.0f), TimeFunction));

		//font2.Load("Assets/Fonts/arial.ttf", 20, BF::Graphics::Fonts::FontAtlasFactory::Language::English);

		/*Math::Vector2 v1 = font.MeasureString("Blue Flame Engine !");
		BF_LOG_INFO("%f, %f", v1.x, v1.y);

		Math::Vector2 v = font2.MeasureString("Blue Flame Engine 22!");
		BF_LOG_INFO("%f, %f", v.x, v.y);*/

		//BF_LOG_WARNING("w: %i, h: %i", fontAtlas->texture->GetTextureData().width, fontAtlas->texture->GetTextureData().height);
		//fontSprite = Sprite(fontAtlas->texture, BF::Math::Vector3(350.0f, 350.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f));

		tileMap.Load("Assets/Maps/TileMap.bfm");
		//spriteAnimation.Load("Assets/Animation/spriteAnimation.bfa");

		//button.AddOnClickListener(DoButtonWork);

		button.SetRectangle(BF::Math::Rectangle(700, 100, 300, 300));
		button2.SetRectangle(BF::Math::Rectangle(900, 100, 200, 50));
		button3.SetRectangle(BF::Math::Rectangle(925, 125, 200, 150));

		button.SetZLayer(4);
		button2.SetZLayer(0);
		button3.SetZLayer(0);

		//renderTarget.Create(800, 600, Texture::Format::R8G8B8, Texture::TextureWrap::ClampToBorder, Texture::TextureFilter::Neatest);
	}

/*	void _2DScene::FixedUpdate()
	{
		//Scene::FixedUpdate();

		//BF_WARNING("TICKS");
		//spriteAnimation.Update();
	}*/

	void DoButtonWork()
	{
		BF_LOG_INFO("lol xD");
	}

	void _2DScene::Update()
	{
		//This is temporary.
		//BF_LOG_INFO("w: %i h: %i", Engine::GetWindow().GetClientWidth(), Engine::GetWindow().GetClientHeight());

		camera.SetProjectionMatrix(Matrix4::Orthographic(0.0f, Engine::GetWindow().GetClientWidth(), 0.0f, Engine::GetWindow().GetClientHeight(), -1.0f, 1.0f));
		Scene::Update();
		camera.Update();

		if (button.IsPressed())
			BF_LOG_INFO("PRESSED");

		if (button2.IsPressed())
			BF_LOG_INFO("PRESSED2");

		if (button3.IsPressed())
			BF_LOG_INFO("PRESSED3");

		//waypoint.Update();

		/*waypoint.GoTo(node);

		if(node.HasArrived())
			waypoint.GoTo(node2);*/

		/*if (checkbox.IsChecked())
			BF_LOG_ERROR("CHECKED !!");
		else
			BF_LOG_WARNING("NOT CHECKED !! ");*/

		//BF_LOG_INFO("%f %f", BF::Input::Mouse::GetPositionRelativeToWindow().x, BF::Input::Mouse::GetPositionRelativeToWindow().y );
		//BF_LOG_INFO("%f %f", BF::Input::Mouse::GetPositionRelativeToScreen().x, BF::Input::Mouse::GetPositionRelativeToScreen().y);
	}

	void _2DScene::Render()
	{
		//renderTarget.Bind();
		BF::Engine::GetContext().Clear(Color(0.5, 0.0f, 0.0f, 1.0f));
		Scene::Render();

		spriteRenderer.Begin(SpriteRenderer::SubmitType::DynamicSubmit, SpriteRenderer::SortingOrder::BackToFront);

		/*spriteRenderer.Render(line);
		spriteRenderer.Render(rp);
		spriteRenderer.Render(sprite2);
		spriteRenderer.Render(line2);
		spriteRenderer.Render(rp2);
		spriteRenderer.Render(rpp);*/
		
		//spriteRenderer.RenderLine(Vector2(0.0f, 300.0f), Vector2(800.0f, 300.0f), 5, Color(1.0f, 0.0f, 0.0f, 1.0f));



		//spriteRenderer.SetScissor(Math::Rectangle(25, 50, 125, 200));
		//spriteRenderer.SetScissor(Math::Rectangle(0, 100, Engine::GetWindow().GetClientWidth(), Engine::GetWindow().GetClientHeight() - 200));
		//spriteRenderer.Render(sprite8);
		//spriteRenderer.SetScissor(Math::Rectangle(0, 0, 1920, 1080));

		//spriteRenderer.End();
		//spriteRenderer.SetScissor(Math::Rectangle(0, 0, 1920, 1080));

		//spriteRenderer.SetScissor(Math::Rectangle(0, 0, 1920, 1080));
		//spriteRenderer.EndScissor();

		//BF::Engine::GetContext().EnableScissor(false);

		/*spriteRenderer.Render(sprite2);
		spriteRenderer.Render(sprite3);
		spriteRenderer.Render(sprite4);
		spriteRenderer.Render(sprite5);
		spriteRenderer.Render(sprite1);*/

		
		//spriteRenderer.Render(sprite6);
		

		//spriteRenderer.RenderLine(Vector2(100.0f, 100.0f), Vector2(200.0f, 100.0f), 5, Vector4(0.0f, 0.0f, 1.0f, 1.0f));						  
		//spriteRenderer.RenderLine(Vector2(100.0f, 400.0f), Vector2(200.0f, 600.0f), 10, Vector4(0.0f, 0.0f, 1.0f, 1.0f));

		//spriteRenderer.Draw(sprite3);
		//spriteRenderer.Draw(sprite4);
		//spriteRenderer.Render(fontSprite);
		//spriteRenderer.RenderText(font, "Blue Flame Engine !", BF::Math::Vector2(0.0f, 0.0f), BF::Graphics::Color(0.0f, 0.0f, 1.0f, 1.0f));
		//spriteRenderer.RenderText(font, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz", BF::Math::Vector2(0.0f, 0.0f), BF::Graphics::Color(0.0f, 0.0f, 1.0f, 1.0f));

		spriteRenderer.Render(text);
		//spriteRenderer.RenderText(font2, "Blue Flame Engine 22!", BF::Math::Vector2(0.0f, 0.0f), BF::Graphics::Color(0.0f, 0.0f, 1.0f, 1.0f));
		
		
		//spriteRenderer.RenderText(*fontAtlas, "a b c d e f g h i j k l m n o p q r s t u v w x y z", BF::Math::Vector3(0.0f, 150.0f, 0.0f), BF::Math::Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		//spriteRenderer.RenderText(*fontAtlas, "A B C D E F G H I J K L MNOPQRSTUVWXYZ", BF::Math::Vector3(0.0f, 250.0f, 0.0f), BF::Math::Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		//spriteRenderer.Draw(sprite5);
		//spriteRenderer.DrawRectangle(BF::Math::Rectangle(1200, 800, 100, 100), BF::Math::Vector4(1.0, 0.0, 1.0, 1.0));

		//spriteAnimation.Render();
		spriteRenderer.End();

		tileMap.Render();
		//renderTarget.Unbind();



		/*
		BF::Engine::GetContext().Clear(Color(0.0, 0.0f, 0.5f, 1.0f));

		Scene::Render();

		spriteRenderer.Begin(SpriteRenderer::SubmitType::DynamicSubmit, SpriteRenderer::SortingOrder::None);
		spriteRenderer.Render(Sprite(&renderTarget.GetTexture2D(), Vector2(700, 0), 0, Color(1.0f)));
		spriteRenderer.End();
		*/
		BF::Engine::GetContext().SwapBuffers();
	}
}