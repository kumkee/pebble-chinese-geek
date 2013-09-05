#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "chical.h"
#include "strtools.h"


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
TextLayer text_hexa_layer;

TextLayer text_gyear_layer;
TextLayer text_gdate_layer;
TextLayer text_cdate_layer;


#define tot_length	144
#define tot_height	168

#define bazi_font	fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IPA_SUBSET_27))
#define ke_font		bazi_font
#define left_margin	5
#define	right_margin	5
#define	top_margin	5
#define	bazi_length	tot_length - bazi_left_margin - right_margin
#define	bazi_height	27
#define ke_length	28
#define ke_height	bazi_height*3 + 5
#define ke_top_margin	top_margin// + bazi_height
#define	bazi_left_margin	ke_length + left_margin

#define hexa_top_margin	top_margin + 3*bazi_height
#define hexa_length	bazi_height
#define hexa_height	2*bazi_height
#define hexa_font	fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HANA_SUBSET_29))

#define gyear_font	fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_VERTICAL_17))
#define gyear_length	18
#define gyear_top_margin	top_margin + ke_height
#define gyear_height	tot_height - gyear_top_margin
#define gyear_left_margin	tot_length - right_margin - gyear_length

#define gdate_font	fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_VERTICAL_14))
#define gdate_length	gyear_length
#define gdate_height	gyear_height
#define gdate_top_margin	gyear_top_margin
#define gdate_left_margin	gyear_left_margin - gdate_length

#define cdate_font	fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IPA_SUB_13))
#define cdate_length	gyear_length
#define cdate_height	gyear_height + 5
#define cdate_top_margin	gyear_top_margin - 8
#define cdate_left_margin	gdate_left_margin - cdate_length


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
  text_layer_set_text(&text_ymdh_layer, "時日月年");

  LayerInit(text_ke_layer, left_margin, ke_top_margin, ke_length, ke_height, ke_font);

  LayerInit(text_hexa_layer, left_margin, hexa_top_margin, hexa_length, hexa_height, hexa_font);

  LayerInit(text_gyear_layer, gyear_left_margin, gyear_top_margin, gyear_length, gyear_height, gyear_font);

  LayerInit(text_gdate_layer, gdate_left_margin, gdate_top_margin, gdate_length, gdate_height, gdate_font);

  LayerInit(text_cdate_layer, cdate_left_margin, cdate_top_margin, cdate_length, cdate_height, cdate_font);

}



void handle_second_tick(AppContextRef ctx, PebbleTickEvent *evt) {

  static char gan_text[] = "YYYMMMDDDHHH";
  static char zhi_text[] = "YYYMMMDDDHHH";
  static char ke_text[]  = "初\n初\n刻";
  static char hex_text[]  = "初";
  static char gyear_text[]  = "3\n1\n0\n2";
  static char gdate_text[]  = "0\n3\ng\nu\nA";
  static char cdate_text[]  = "閏\n十\n一\n月\n初\n三";
  //static char cdate_text2[25]  = "閏十一月初三";

  static bool is_ganzhi_drawn = false;
  static bool is_ke_drawn = false;
  static bool is_gyear_drawn = false;
  static bool is_gdate_drawn = false;
  static bool is_cdate_drawn = false;

  if( ((evt->units_changed & HOUR_UNIT) && evt->tick_time->tm_hour%2==1) || !is_ganzhi_drawn )
  {	
	GenerateCDateText(evt->tick_time, cdate_text, gan_text, zhi_text, true);
	text_layer_set_text(&text_gan_layer, gan_text);
	text_layer_set_text(&text_zhi_layer, zhi_text);

	str_verticize_zh(cdate_text);
	text_layer_set_text(&text_cdate_layer, cdate_text);

	is_ganzhi_drawn = true;
  }

  if( ((evt->units_changed & MINUTE_UNIT) && evt->tick_time->tm_min%15==0) || !is_ke_drawn )
  {	
	GenerateKeText(evt->tick_time, ke_text);
	text_layer_set_text(&text_ke_layer, ke_text);
	is_ke_drawn = true;
  }

  if(IfNewHexa(evt->tick_time)) {
	GenerateHexagram(evt->tick_time, hex_text);
	text_layer_set_text(&text_hexa_layer, hex_text);
  }

  if( (evt->units_changed & YEAR_UNIT) || !is_gyear_drawn) {
	string_format_time(gyear_text, 7, "%Y", evt->tick_time);
	str_verticize(gyear_text);
	text_layer_set_text(&text_gyear_layer, gyear_text);
  }

  if( (evt->units_changed & DAY_UNIT) || !is_gdate_drawn) {
	string_format_time(gdate_text, 9, "%b%d", evt->tick_time);
	str_verticize(gdate_text);
	text_layer_set_text(&text_gdate_layer, gdate_text);
  }

}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_second_tick,
      .tick_units = SECOND_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
