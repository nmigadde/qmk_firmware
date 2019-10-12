/* Copyright 2019 Jason Williams (Wilba)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// Allow keyboard level code to reuse VIA's EEPROM magic so firmware can
// detect invalid EEPROM data after a firmware change of EEPROM usage.
//
// This magic number can be changed (incremented) every time keyboard level
// code changes its EEPROM usage, so new firmware install will trigger
// invalid EEPROM handling and skip loading (garbage) from EEPROM.
#ifndef VIA_EEPROM_EXTRA_MAGIC
#	define VIA_EEPROM_EXTRA_MAGIC 0x0000
#endif

// This is the VIA magic. Remains relatively constant, since there
// is VIA_EEPROM_VERSION as well.
#define VIA_EEPROM_MAGIC (0x4521 ^ VIA_EEPROM_EXTRA_MAGIC)

// Keyboard level code can change where VIA stores the magic.
// This gets changed if/when QMK's EEPROM usage extends.
#ifndef VIA_EEPROM_MAGIC_ADDR
#	define VIA_EEPROM_MAGIC_ADDR 34
#endif

// Bump this every time we change what we store
// This will automatically reset the EEPROM with defaults
// and avoid loading invalid data from the EEPROM.
// This is (typically) kept in sync with VIA_PROTOCOL_VERSION
// so that (for example) a firmware upgrade with changes to
// keycodes will invalidate existing keymaps in EEPROM.
#define VIA_EEPROM_VERSION 0x08

#define VIA_EEPROM_VERSION_ADDR (VIA_EEPROM_MAGIC_ADDR+2)

// This is changed only when the command IDs change,
// so VIA Configurator can detect compatible firmware. 
#define VIA_PROTOCOL_VERSION 0x0008

enum via_command_id
{
	id_get_protocol_version = 0x01, // always 0x01
	id_get_keyboard_value,
	id_set_keyboard_value,
	id_dynamic_keymap_get_keycode,
	id_dynamic_keymap_set_keycode,
	id_dynamic_keymap_reset,
	id_backlight_config_set_value,
	id_backlight_config_get_value,
	id_backlight_config_save,
	id_eeprom_reset,
	id_bootloader_jump,
	id_dynamic_keymap_macro_get_count,
	id_dynamic_keymap_macro_get_buffer_size,
	id_dynamic_keymap_macro_get_buffer,
	id_dynamic_keymap_macro_set_buffer,
	id_dynamic_keymap_macro_reset,
	id_dynamic_keymap_get_layer_count,
	id_dynamic_keymap_get_buffer,
	id_dynamic_keymap_set_buffer,
	id_unhandled = 0xFF,
};

enum via_keyboard_value_id
{
  id_uptime = 0x01,
  id_firmware_version
};

// Can't use SAFE_RANGE here, it might change if someone adds
// new values to enum quantum_keycodes.
// Need to keep checking 0x5F10 is still in the safe range.
// TODO: merge this into quantum_keycodes
// Backlight keycodes are in range 0x5F00-0x5F0F
enum via_keycodes {
	FN_MO13 = 0x5F10,
	FN_MO23,
	MACRO00,
	MACRO01,
	MACRO02,
	MACRO03,
	MACRO04,
	MACRO05,
	MACRO06,
	MACRO07,
	MACRO08,
	MACRO09,
	MACRO10,
	MACRO11,
	MACRO12,
	MACRO13,
	MACRO14,
	MACRO15,
};

enum user_keycodes {
  USER00 = 0x5F80,
  USER01,
  USER02,
  USER03,
  USER04,
  USER05,
  USER06,
  USER07,
  USER08,
  USER09,
  USER10,
  USER11,
  USER12,
  USER13,
  USER14,
  USER15,
};

// Can be called in an overriding via_init_kb() to test if keyboard level code usage of
// EEPROM is invalid and use/save defaults.
bool via_eeprom_is_valid(void);

// Sets VIA/keyboard level usage of EEPROM to valid/invalid
// Keyboard level code (eg. via_init_kb()) should not call this
void via_eeprom_set_valid(bool valid);

// Flag QMK and VIA/keyboard level EEPROM as invalid.
// Used in bootmagic_lite() and VIA command handler.
// Keyboard level code should not need to call this.
void via_eeprom_reset(void);

// Called by QMK core to initialize dynamic keymaps etc.
void via_init(void);

// Called by QMK core to process VIA-specific keycodes.
bool process_record_via(uint16_t keycode, keyrecord_t *record);

