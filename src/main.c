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
TextLayer text_zhi_layer;
TextLayer text_ymdh_layer;
TextLayer text_ke_layer;


#define bazi_font	fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IPA_SUBSET_29))
#define ke_font		fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IPA_SUBSET_18))
#define left_margin	5
#define	right_margin	5
#define	top_margin	5
#define	bazi_length	144 - bazi_left_margin - right_margin
#define	bazi_height	29
#define ke_length	19
#define ke_height	bazi_height*2
#define ke_top_margin	1.8*top_margin + bazi_height
#define	bazi_left_margin	ke_length + left_margin
#define LayerInit(Layer, X, Y, L, H, Font) \
	text_layer_init(&Layer, window.layer.frame); \
	text_layer_set_text_color(&Layer, GColorWhite); \
	text_layer_set_background_color(&Layer, GColorClear); \
	layer_set_frame(&Layer.layer, GRect(X, Y, L, H)); \
	text_layer_set_font(&Layer, Font); \
	layer_add_child(&window.layer, &Layer.layer);

void handle_init(AppContextRef ctx) {

  window_init(&window, "Chinese Geek");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  resource_init_current_app(&APP_RESOURCES);

  LayerInit(text_gan_layer, bazi_left_margin, top_margin, bazi_length, bazi_height+3, bazi_font);

  LayerInit(text_zhi_layer, bazi_left_margin, top_margin+bazi_height, bazi_length, bazi_height+3, bazi_font);

  LayerInit(text_ymdh_layer, bazi_left_margin, top_margin+2*bazi_height, bazi_length, bazi_height+3, bazi_font);

  LayerInit(text_ke_layer, left_margin, ke_top_margin, ke_length, ke_height, ke_font);

}



void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *evt) {

  static char ccd_text[] = "LLLTTTDDDYYYCCCDDD";
  static char gan_text[] = "YYYMMMDDDHHH";
  static char zhi_text[] = "YYYMMMDDDHHH";
  static char ke_text[]  = "初\n初\n刻";

  static bool is_ganzhi_drawn = false;
  static bool is_ke_drawn = false;


  if(!is_ganzhi_drawn)	text_layer_set_text(&text_ymdh_layer, "時日月年");
  if( ((evt->units_changed & HOUR_UNIT) && evt->tick_time->tm_hour%2==1) || !is_ganzhi_drawn )
  {	
	GenerateCDateText(evt->tick_time, ccd_text, gan_text, zhi_text, ZhDisplay);
	text_layer_set_text(&text_gan_layer, gan_text);
	text_layer_set_text(&text_zhi_layer, zhi_text);
	is_ganzhi_drawn = true;
  }

  if( ((evt->units_changed & MINUTE_UNIT) && evt->tick_time->tm_min%15==0) || !is_ke_drawn )
  {	
	GenerateKeText(evt->tick_time, ke_text);
	text_layer_set_text(&text_ke_layer, ke_text);
	is_ke_drawn = true;
  }

}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
