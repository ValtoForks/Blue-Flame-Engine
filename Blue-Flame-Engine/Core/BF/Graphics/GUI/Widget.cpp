#include "Widget.h"
#include "BF/Graphics/GUI/StyleSheet.h"
#include "BF/Input/Mouse.h"
#include "BF/System/Log.h"
#include "BF/Math/Math.h"

namespace BF
{
	namespace Graphics
	{
		namespace GUI
		{
			using namespace std;
			using namespace BF::Math;
			using namespace BF::Input;
			using namespace BF::Graphics::Renderers;

			Widget::Widget() :
				currentSprite(nullptr), OnClickCallBack(nullptr), callBackPointer(nullptr), currentState(0), mouseNotPressedOnWidget(false), hovered(false), pressed(false), pressedAndReleased(false)
			{
			}

			Widget::~Widget()
			{
			}

			void Widget::Initialize(Renderers::SpriteRenderer& spriteRenderer, int zLayer)
			{
				this->spriteRenderer = &spriteRenderer;
				SetZLayer(zLayer);
			}

			void Widget::Load(const StyleSheet& StyleSheet, const string& widgetName)
			{
				widgetData = StyleSheet.GetWidget(widgetName);
				currentSprite = &widgetData.sprites[0];
			}

			void Widget::AddOnClickListener(void(*OnClickCallBack)(void*), void* callBackPointers)
			{
				this->OnClickCallBack = OnClickCallBack;
				this->callBackPointer = callBackPointers;
			}

			void Widget::SetPosition(const Vector2& position)
			{
				for (unsigned int i = 0; i < BF_WIDGET_DATA_SPRITES_LENGTH; i++)
					widgetData.sprites[i].SetPosition(position);
			}

			void Widget::SetZLayer(int zLayer)
			{
				for (unsigned int i = 0; i < BF_WIDGET_DATA_SPRITES_LENGTH; i++)
					widgetData.sprites[i].zLayer = zLayer;
			}

			void Widget::SetTextAlignment(WidgetData::TextAlignment textAlignment)
			{
				switch (textAlignment)
				{
				case WidgetData::TextAlignment::TopLeft:
				{
					widgetData.textPosition = GetPosition();
					break;
				}
				case WidgetData::TextAlignment::TopCenter:
				{
					break;
				}
				case WidgetData::TextAlignment::TopRight:
				{
					break;
				}
				case WidgetData::TextAlignment::MiddleLeft:
				{
					break;
				}
				case WidgetData::TextAlignment::MiddleCenter:
				{
					break;
				}
				case WidgetData::TextAlignment::MiddleRight:
				{
					break;
				}
				case WidgetData::TextAlignment::BottomLeft:
				{
					break;
				}
				case WidgetData::TextAlignment::BottomCenter:
				{
					break;
				}
				case WidgetData::TextAlignment::BottomRight:
				{
					break;
				}
				default:
					break;
				}
			}

			void Widget::SetRectangle(const Math::Rectangle& rectangle)
			{
				Math::Rectangle temp = rectangle;
				temp.width = Max(widgetData.minWidth, rectangle.width);
				temp.height = Max(widgetData.minHeight, rectangle.height);

				for (unsigned int i = 0; i < BF_WIDGET_DATA_SPRITES_LENGTH; i++)
					widgetData.sprites[i].SetRectangle(temp);
			}

			bool Widget::IsMouseOnWidget()
			{
				//if (Mouse::IsInsideWindowClient())
				{
					if (Mouse::GetPosition().x >= currentSprite->GetRectangle().x && Mouse::GetPosition().x <= currentSprite->GetRectangle().x + currentSprite->GetRectangle().width &&
						Mouse::GetPosition().y >= currentSprite->GetRectangle().y && Mouse::GetPosition().y <= currentSprite->GetRectangle().y + currentSprite->GetRectangle().height)
						return true;
				}

				return false;
			}

			void Widget::FireAction()
			{
				if (hovered && pressed && !Mouse::IsButtonPressed(Mouse::Button::Left))
				{
					pressed = false;
					currentSprite = &widgetData.sprites[currentState + 0];

					if (OnClickCallBack != nullptr && callBackPointer != nullptr)
						OnClickCallBack(callBackPointer);

					pressedAndReleased = true;
				}
			}

			void Widget::SetCurrentSpriteToNormal()
			{
				currentSprite = &widgetData.sprites[currentState + 0];
			}

			void Widget::Update()
			{
				pressedAndReleased = false;

				hovered = IsMouseOnWidget();

				if (hovered)
					currentSprite = &widgetData.sprites[currentState + 1];
				else
					currentSprite = &widgetData.sprites[currentState + 0];

				if (!hovered && Mouse::IsButtonPressed(Mouse::Button::Left))
					mouseNotPressedOnWidget = true;

				if (!mouseNotPressedOnWidget && hovered && Mouse::IsButtonPressed(Mouse::Button::Left))
				{
					currentSprite = &widgetData.sprites[currentState + 2];
					pressed = true;
				}

				FireAction();

				if (!Mouse::IsButtonPressed(Mouse::Button::Left))
				{
					mouseNotPressedOnWidget = false;
					pressed = false;
				}
			}

			void Widget::Render()
			{
				spriteRenderer->Render(*currentSprite);

				if (widgetData.renderText)
				{
					//spriteRenderer->RenderText(*widgetData.font, "", )
				}
				
			}

			void Widget::SwitchState()
			{
				currentState == 0 ? currentState = 4 : currentState = 0;
			}
		}
	}
}