#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "lylunar.h"

const bool ZhDisplay = true;


#define MY_UUID { 0x42, 0x6F, 0x1A, 0x53, 0x91, 0x1E, 0x4C, 0x29, 0x8D, 0xFA, 0x01, 0xD6, 0xFE, 0x9A, 0x46, 0xB4 }
PBL_APP_INFO(MY_UUID,
             "Chinese Geek", "kumkee",
             0, 1, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;

TextLayer text_gan_layer;


void handle_init(AppContextRef ctx) {

  window_init(&window, "Chinese Geek");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  resource_init_current_app(&APP_RESOURCES);


  text_layer_init(&text_gan_layer, window.layer.frame);
  text_layer_set_text_color(&text_gan_layer, GColorWhite);
  text_layer_set_background_color(&text_gan_layer, GColorClear);
  layer_set_frame(&text_gan_layer.layer, GRect(8, 8, 128, 56));
  text_layer_set_font(&text_gan_layer, 
	fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IPA_SUBSET_32)));
  layer_add_child(&window.layer, &text_gan_layer.layer);
}



void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *evt) {

  static char ccd_text[] = "LLLTTTDDDYYYCCCDDD";
  static char gan_text[] = "YYYMMMDDDHHH";
  static char zhi_text[] = "YYYMMMDDDHHH";

  GenerateCDateText(evt->tick_time, ccd_text, gan_text, zhi_text, ZhDisplay);
  text_layer_set_text(&text_gan_layer, gan_text);

}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = HOUR_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
