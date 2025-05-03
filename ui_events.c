// SquareLine Studio version: SquareLine Studio 1.5.1
// LVGL version: 9.1.0
// Project name: SquareLine_Project

#include "ui.h"
#include <stdio.h>
#include "predict.h"
#include <Arduino.h>
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
// Global değişkenler
static float val1 = 0.0f;
static float val2 = 0.0f;
static float val3 = 0.0f;
static float val4 = 0.0f;

// Switch’leri tek seçimli hale getiren callback’ler
void change_1(lv_event_t* e) {
	lv_obj_remove_state(ui_chb2, LV_STATE_CHECKED);
	lv_obj_remove_state(ui_chb3, LV_STATE_CHECKED);
	lv_obj_remove_state(ui_chb4, LV_STATE_CHECKED);
}

void change_2(lv_event_t* e) {
	lv_obj_remove_state(ui_chb1, LV_STATE_CHECKED);
	lv_obj_remove_state(ui_chb3, LV_STATE_CHECKED);
	lv_obj_remove_state(ui_chb4, LV_STATE_CHECKED);
}

void change_3(lv_event_t* e) {
	lv_obj_remove_state(ui_chb1, LV_STATE_CHECKED);
	lv_obj_remove_state(ui_chb2, LV_STATE_CHECKED);
	lv_obj_remove_state(ui_chb4, LV_STATE_CHECKED);
}

void change_4(lv_event_t* e) {
	lv_obj_remove_state(ui_chb1, LV_STATE_CHECKED);
	lv_obj_remove_state(ui_chb2, LV_STATE_CHECKED);
	lv_obj_remove_state(ui_chb3, LV_STATE_CHECKED);
}
// Değer artırma/azaltma fonksiyonu
void value_change(bool status, float step) {

	char buf[16];
	bool sw1 = lv_obj_has_state(ui_chb1, LV_STATE_CHECKED);
	bool sw2 = lv_obj_has_state(ui_chb2, LV_STATE_CHECKED);
	bool sw3 = lv_obj_has_state(ui_chb3, LV_STATE_CHECKED);
	bool sw4 = lv_obj_has_state(ui_chb4, LV_STATE_CHECKED);


	if (sw1) {
		if (status) {
			val1 += step;
		} else {
			// Sadece step kadar azalt, ama 0’ın altına inme
			val1 = max(0.0f, val1 - step);
		}
		snprintf(buf, sizeof(buf), "%.1f", val1);
		lv_label_set_text(ui_val1, buf);

	} else if (sw2) {
		if (status) {
			val2 += step;
		} else {
			val2 = max(0.0f, val2 - step);
		}
		snprintf(buf, sizeof(buf), "%.1f", val2);
		lv_label_set_text(ui_val2, buf);

	} else if (sw3) {
		if (status) {
			val3 += step;
		} else {
			val3 = max(0.0f, val3 - step);
		}
		snprintf(buf, sizeof(buf), "%.1f", val3);
		lv_label_set_text(ui_val3, buf);
	} else if (sw4) {
		if (status) {
			val4 += step;
		} else {
			val4 = max(0.0f, val4 - step);
		}
		snprintf(buf, sizeof(buf), "%.1f", val4);
		lv_label_set_text(ui_val4, buf);
	}
}

// Artı buton click handler
void plus_button(lv_event_t* e) {
	value_change(true, 0.1f);
}

// Eksi buton click handler
void minus_button(lv_event_t* e) {
	value_change(false, 0.1f);
}

// Artı buton click handler
void plus_button_long(lv_event_t* e) {
	// value_change(true, 0.2f);
	// delay(100);
}

// Eksi buton click handler
void minus_button_long(lv_event_t* e) {
	// value_change(false, 0.2f);
	// delay(100);
}
void calculate(lv_event_t* e) {
	const char* species_names[] = { "Setosa", "Versicolor", "Virginica" };
	lv_label_set_text(ui_predict, species_names[predict(val1, val2, val3, val4)]);
}
