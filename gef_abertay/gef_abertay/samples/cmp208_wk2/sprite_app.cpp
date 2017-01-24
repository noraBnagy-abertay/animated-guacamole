#include "sprite_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <maths/math_utils.h>
#include <input/sony_controller_input_manager.h>
#include <system/debug_log.h>


SpriteApp::SpriteApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	input_manager_(NULL),
	font_(NULL)
{
}

void SpriteApp::Init()
{
	// create a sprite renderer to draw your sprites
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	InitFont();

	my_sprite_.set_position(platform_.width()*0.5f, platform_.height()*0.5f, 0.0f);
	my_sprite_.set_width(32.0f);
	my_sprite_.set_height(32.0f);

	// create the input manager

	input_manager_ = gef::InputManager::Create(platform_);
}

void SpriteApp::CleanUp()
{
	CleanUpFont();

	// destroy the input manager
	delete input_manager_;
	input_manager_ = NULL;

	// destroy sprite renderer once you've finished using it
	delete sprite_renderer_;
	sprite_renderer_ = NULL;
}

bool SpriteApp::Update(float frame_time)
{
	leftAxisAngle = 0;
	// frame rate = 1 second / frame_delta_time
	fps_ = 1.0f / frame_time;

	// get a copy of the current position of the sprite
	gef::Vector4 sprite_position = my_sprite_.position();

	// get the data from the latest input devices
	if (input_manager_)
	{
		input_manager_->Update();

		//get controller input data
		gef::SonyControllerInputManager*controller_input = input_manager_->controller_input();
		if (controller_input)
		{
			// read controller data
			const gef::SonyController* controller = controller_input->GetController(0);
			

			if (controller)
			{
				// get the x axis for the left stick
				float left_x_axis = controller->left_stick_x_axis();

				gef::DebugOut(" LX: %f \n", left_x_axis);

				// get the y axis for the left stick
				float left_y_axis = controller->left_stick_y_axis();

				gef::DebugOut(" LY: %f \n", left_y_axis);

				// get the x axis for the right stick

				float right_x_axis = controller->right_stick_x_axis();

				gef::DebugOut(" RX: %f \n", right_x_axis);

				//get the y axis for the right stick

				float right_y_axis = controller->right_stick_y_axis();

				gef::DebugOut(" RY: %f \n", right_y_axis);

				// Angle stuff

				if ((left_x_axis > 0) && (left_y_axis < 0))
				{
					leftAxisAngle = ( std::atan((left_x_axis / -left_y_axis)*180)/ 3.1415926535);
				}
				else if ((left_x_axis > 0) && (left_y_axis > 0))
				{
					leftAxisAngle = 180 - (std::atan((left_x_axis / left_y_axis) * 180) / 3.1415926535);
				}
				else if ((left_x_axis < 0) && (left_y_axis > 0))
				{
					leftAxisAngle = 180 + (std::atan((left_x_axis / -left_y_axis) * 180) / 3.1415926535);
				}
				else if ((left_x_axis < 0) && (left_y_axis < 0))
				{
					leftAxisAngle = 360 - (std::atan((left_x_axis / left_y_axis) * 180) / 3.1415926535);
				}

				else if ((left_x_axis == 0) && (left_y_axis == 1))
				{
					leftAxisAngle = 0;
				}
				else if ((left_x_axis == 1) && (left_y_axis == 0))
				{
					leftAxisAngle = 90;
				}
				else if ((left_x_axis == 0) && (left_y_axis == -1))
				{
					leftAxisAngle = 180;
				}
				else if ((left_x_axis == -1) && (left_y_axis == 0))
				{
					leftAxisAngle = 270;
				}
				// Two button terminates the program

				if (((controller->buttons_down()) & (gef_SONY_CTRL_START | gef_SONY_CTRL_SELECT)) == (gef_SONY_CTRL_START | gef_SONY_CTRL_SELECT))
				{
					// the main function is int so if returns 0 it closes the program.
					return 0;
				}
				// test for x button
				if (controller->buttons_down() & gef_SONY_CTRL_CROSS)
				{
					//do sth
					gef::DebugOut("x is held down\n");
				}
				// test for triangle button
				if (controller->buttons_down() & gef_SONY_CTRL_TRIANGLE)
				{
					sprite_position.set_x(430);
					//do sth
					gef::DebugOut("Triangle is held down\n");
				}
				// test for o button
				if (controller->buttons_down() & gef_SONY_CTRL_CIRCLE)
				{
					//do sth
					gef::DebugOut("O is held down\n");
				}
				// test for rectangle button
				if (controller->buttons_down() & gef_SONY_CTRL_SQUARE)
				{
					//do sth
					gef::DebugOut("square is held down\n");
				}
			}
		}
	}



	// move the sprite along the x-axis
	

	// update the x-axis on the COPY of the current position
	sprite_position.set_x(430);


	my_sprite_.set_rotation(my_sprite_.rotation() + leftAxisAngle);

	// update the sprite with the new position
	my_sprite_.set_position(sprite_position);

	return true;
}

void SpriteApp::Render()
{
	// draw all sprites between the Begin() and End() calls
	sprite_renderer_->Begin();

	// draw my sprite here
	sprite_renderer_->DrawSprite(my_sprite_);

	DrawHUD();
	sprite_renderer_->End();
}


void SpriteApp::InitFont()
{
	// create a Font object and load in the comic_sans font data
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void SpriteApp::CleanUpFont()
{
	// destroy the Font object
	delete font_;
	font_ = NULL;
}

void SpriteApp::DrawHUD()
{
	if(font_)
	{
		// display frame rate
		font_->RenderText(
			sprite_renderer_,						// sprite renderer to draw the letters
			gef::Vector4(650.0f, 510.0f, -0.9f),	// position on screen
			1.0f,									// scale
			0xffffffff,								// colour ABGR
			gef::TJ_LEFT,							// justification
			"FPS: %.1f xpos: %.1f",							// string of text to render
			fps_,									// any variables used in formatted text string http://www.cplusplus.com/reference/cstdio/printf/
			my_sprite_.position().x()
			);

		// display angle
		font_->RenderText(
			sprite_renderer_,						// sprite renderer to draw the letters
			gef::Vector4(150.0f, 510.0f, -0.9f),	// position on screen
			1.0f,									// scale
			0xffffffff,								// colour ABGR
			gef::TJ_LEFT,							// justification
			"Angle: %.1f",							// string of text to render
			leftAxisAngle								// any variables used in formatted text string http://www.cplusplus.com/reference/cstdio/printf/

			);
	}
}

