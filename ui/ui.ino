//Elecrow DIS08070H board-template main source file
#include<Arduino.h>
//#define ESP_PLATFORM
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <lvgl.h>
#include <ui.h>
#include <expt.hpp>
#include <engine.hpp>  // include engine header
#include <gui/gui_callbacks.hpp>  // include GUI callback declarations

/*Don't forget to set Sketchbook location in File/Preferences to the path of your UI project (the parent foder of this INO file)*/


enum BoardConstants { TFT_BL=2, LVGL_BUFFER_RATIO=6 };


class LGFX : public lgfx::LGFX_Device
{
public:
  lgfx::Bus_RGB     _bus_instance;
  lgfx::Panel_RGB   _panel_instance;
  LGFX (void)
  {
    {
      auto cfg = _bus_instance.config();
      cfg.panel = &_panel_instance;

      cfg.pin_d0  = GPIO_NUM_15; // B0
      cfg.pin_d1  = GPIO_NUM_7;  // B1
      cfg.pin_d2  = GPIO_NUM_6;  // B2
      cfg.pin_d3  = GPIO_NUM_5;  // B3
      cfg.pin_d4  = GPIO_NUM_4;  // B4

      cfg.pin_d5  = GPIO_NUM_9;  // G0
      cfg.pin_d6  = GPIO_NUM_46; // G1
      cfg.pin_d7  = GPIO_NUM_3;  // G2
      cfg.pin_d8  = GPIO_NUM_8;  // G3
      cfg.pin_d9  = GPIO_NUM_16; // G4
      cfg.pin_d10 = GPIO_NUM_1;  // G5

      cfg.pin_d11 = GPIO_NUM_14; // R0
      cfg.pin_d12 = GPIO_NUM_21; // R1
      cfg.pin_d13 = GPIO_NUM_47; // R2
      cfg.pin_d14 = GPIO_NUM_48; // R3
      cfg.pin_d15 = GPIO_NUM_45; // R4

      cfg.pin_henable = GPIO_NUM_41;
      cfg.pin_vsync   = GPIO_NUM_40;
      cfg.pin_hsync   = GPIO_NUM_39;
      cfg.pin_pclk    = GPIO_NUM_0;
      cfg.freq_write  = 15000000;

      cfg.hsync_polarity    = 0;
      cfg.hsync_front_porch = 40;
      cfg.hsync_pulse_width = 48;
      cfg.hsync_back_porch  = 40;

      cfg.vsync_polarity    = 0;
      cfg.vsync_front_porch = 1;
      cfg.vsync_pulse_width = 31;
      cfg.vsync_back_porch  = 13;

      cfg.pclk_active_neg   = 1;
      cfg.de_idle_high      = 0;
      cfg.pclk_idle_high    = 0;

      _bus_instance.config(cfg);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.memory_width  = 800;
      cfg.memory_height = 480;
      cfg.panel_width  = 800;
      cfg.panel_height = 480;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      _panel_instance.config(cfg);
    }
    _panel_instance.setBus(&_bus_instance);
    setPanel(&_panel_instance);
  }
};


LGFX lcd;

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 800;
static const uint16_t screenHeight = 480;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf [screenWidth * screenHeight / LVGL_BUFFER_RATIO];


#include "touch.h"


#if LV_USE_LOG != 0
/* Serial debugging */
void my_print (const char * buf)
{
    Serial.printf( buf );
    Serial.flush();
}
#endif


/* Display flushing */
void my_disp_flush (lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
    lcd.pushImageDMA( area->x1, area->y1, w, h, (lgfx::rgb565_t*) &color_p->full );
#else
    lcd.pushImageDMA( area->x1, area->y1, w, h, (lgfx::rgb565_t*) &color_p->full );
#endif
    lv_disp_flush_ready( disp );
}


/*Read the touchpad*/
void my_touchpad_read (lv_indev_drv_t * indev_driver, lv_indev_data_t * data)
{
    if ( touch_has_signal() ) {
        if ( touch_touched() ) {
            data->state = LV_INDEV_STATE_PR;
            //Set the coordinates
            data->point.x = touch_last_x;
            data->point.y = touch_last_y;
            //Serial.print( "Data x :" );
            //Serial.println( touch_last_x );
            //Serial.print( "Data y :" );
            //Serial.println( touch_last_y );
        }
        else if ( touch_released() ) {
            data->state = LV_INDEV_STATE_REL;
        }
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }
    delay(15);
}


SensorManager sensorManager;  // Create SensorManager instance
DataBundleManager dataBundleManager; // Create DataBundleManager instance
GuiManager guiManager(sensorManager, dataBundleManager);  // Create GUI manager instance

// Global GUI screen switching functions for use by GUI components
void switchToMenu() {
    guiManager.switchContent(GuiState::MENU);
}

void switchToWiki() {
    guiManager.switchContent(GuiState::WIKI);
}

void switchToVisualization() {
    guiManager.switchContent(GuiState::VISUALIZATION);
}

void switchToDataBundleSelection() {
    guiManager.switchContent(GuiState::DATA_BUNDLE_SELECTION);
}

void switchToCrashScreen(const std::string &reason) {
    guiManager.showCrashScreen(reason);
}

void switchToCreditsScreen() {
    guiManager.switchContent(GuiState::CREDITS);
}

void switchToAppSelectionScreen() {
    guiManager.switchContent(GuiState::APP_SELECTION);
}

void switchToCommunicationSelectionScreen() {
    guiManager.switchContent(GuiState::COMMUNICATION_SELECTION);
}

void setup ()
{
    //Serial.begin( 115200 ); /* prepare for possible serial debug */
    //Serial.println( "Starting setup..." );
    initLogger();
    delay( 10 );

    //Init Display
    lcd.begin();
    lcd.fillScreen( TFT_BLACK );
    delay( 200 );

    lv_init();

    delay( 100 );
    touch_init();

    //screenWidth = lcd.width();
    //screenHeight = lcd.height();
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / LVGL_BUFFER_RATIO );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    ui_init(); 
    lv_timer_handler();

    // Initialize the GUI manager
    if(!guiManager.init())  // Optionally pass config file
    {
        return;
    }


    // Wait a moment to show the boot screen
    delay(2000);
    #define VISENSORS_DEBUG
    #ifndef VISENSORS_DEBUG
    switchToAppSelectionScreen(); // Start in app selection screen
    #else
    switchToWiki(); // Start in wiki screen for debugging
    #endif
    
    //splashMessage("Hello from Elecrow DIS08070H!");
    Serial.println( "Setup done" );
}

void loop ()
{
    // Redraw GUI based on current state
    guiManager.redraw();
}