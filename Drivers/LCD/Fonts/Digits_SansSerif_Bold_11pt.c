// 
//  Font data for Microsoft Sans Serif 11pt
// 

#include "NGL.h"

// Character bitmaps for Microsoft Sans Serif 11pt
const uint8_t DigitSansSerif11ptBitmaps[] =
{
	// @0 ' ' (8 pixels wide)
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //

	// @12 '0' (8 pixels wide)
	0x3C, //   ####
	0x66, //  ##  ##
	0x66, //  ##  ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0x66, //  ##  ##
	0x66, //  ##  ##
	0x3C, //   ####

	// @24 '1' (8 pixels wide)
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x78, //  ####
	0x18, //    ##

	// @36 '2' (8 pixels wide)
	0xFF, // ########
	0xC0, // ##
	0x60, //  ##
	0x30, //   ##
	0x18, //    ##
	0x0C, //     ##
	0x06, //      ##
	0x03, //       ##
	0x03, //       ##
	0xC3, // ##    ##
	0x66, //  ##  ##
	0x3C, //   ####

	// @48 '3' (8 pixels wide)
	0x3C, //   ####
	0x66, //  ##  ##
	0xC3, // ##    ##
	0x03, //       ##
	0x03, //       ##
	0x06, //      ##
	0x1C, //    ###
	0x06, //      ##
	0x03, //       ##
	0xC3, // ##    ##
	0x66, //  ##  ##
	0x3C, //   ####

	// @60 '4' (8 pixels wide)
	0x06, //      ##
	0x06, //      ##
	0x06, //      ##
	0xFF, // ########
	0xC6, // ##   ##
	0x66, //  ##  ##
	0x36, //   ## ##
	0x36, //   ## ##
	0x1E, //    ####
	0x0E, //     ###
	0x0E, //     ###
	0x06, //      ##

	// @72 '5' (8 pixels wide)
	0x3C, //   ####
	0x66, //  ##  ##
	0xC3, // ##    ##
	0x03, //       ##
	0x03, //       ##
	0x03, //       ##
	0xE6, // ###  ##
	0xFC, // ######
	0xC0, // ##
	0xC0, // ##
	0xC0, // ##
	0xFF, // ########

	// @84 '6' (8 pixels wide)
	0x3C, //   ####
	0x66, //  ##  ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0xE6, // ###  ##
	0xFC, // ######
	0xC0, // ##
	0xC3, // ##    ##
	0x66, //  ##  ##
	0x3C, //   ####

	// @96 '7' (8 pixels wide)
	0x18, //    ##
	0x18, //    ##
	0x18, //    ##
	0x0C, //     ##
	0x0C, //     ##
	0x0C, //     ##
	0x06, //      ##
	0x06, //      ##
	0x06, //      ##
	0x03, //       ##
	0x03, //       ##
	0xFF, // ########

	// @108 '8' (8 pixels wide)
	0x3C, //   ####
	0x66, //  ##  ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0x66, //  ##  ##
	0x3C, //   ####
	0x66, //  ##  ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0x66, //  ##  ##
	0x3C, //   ####

	// @120 '9' (8 pixels wide)
	0x3C, //   ####
	0x66, //  ##  ##
	0xC3, // ##    ##
	0x03, //       ##
	0x3F, //   ######
	0x67, //  ##  ###
	0xC3, // ##    ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0xC3, // ##    ##
	0x66, //  ##  ##
	0x3C, //   ####

	// @132 ':' (8 pixels wide)
	0x30, //   ##
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x00, //
	0x30, //   ##
	0x00, //
	0x00, //
	0x00, //
	0x00, //
};


// Character descriptors for Microsoft Sans Serif 12pt
// { [Char width in bits], [Offset into microsoftSansSerif_12ptCharBitmaps in bytes] }
const FONT_CHAR_INFO DigitSansSerif11ptDescriptors[] =
{
	{8, 0}, 		//
	{0, 0}, 		// !
	{0, 0}, 		// "
	{0, 0}, 		// #
	{0, 0}, 		// $
	{0, 0}, 		// %
	{0, 0}, 		// &
	{0, 0}, 		// '
	{0, 0}, 		// (
	{0, 0}, 		// )
	{0, 0}, 		// *
	{0, 0}, 		// +
	{0, 0}, 		// ,
	{0, 0}, 		// -
	{0, 0}, 		// .
	{0, 0}, 		// /
	{8, 12}, 		// 0
	{8, 24}, 		// 1
	{8, 36}, 		// 2
	{8, 48}, 		// 3
	{8, 60}, 		// 4
	{8, 72}, 		// 5
	{8, 84}, 		// 6
	{8, 96}, 		// 7
	{8, 108}, 		// 8
	{8, 120}, 		// 9
	{8, 132}, 		// :
};


// Font information for Microsoft Sans Serif 11pt
const FONT_INFO DigitSansSerif11ptFontInfo =
{
	12,		//  Character height
	1, 		//  Character widht
	' ', 	//  Start character
	':', 	//  End character
	2,    	//  Width, in pixels, of space character
	DigitSansSerif11ptDescriptors, //  Character descriptor array
	DigitSansSerif11ptBitmaps, //  Character bitmap array
};

