// 
//  Font data for Microsoft Sans Serif 10pt
// 

#include "NGL.h"

// Character bitmaps for Microsoft Sans Serif 10pt
// Height = 10 bit
const uint8_t microsoftSansSerif_8ptBitmaps[] =
{
	// @0 '0' (5 pixels wide)
	0x70, //  ### 
	0x88, // #   #
	0x88, // #   #
	0x88, // #   #
	0x88, // #   #
	0x88, // #   #
	0x88, // #   #
	0x88, // #   #
	0x88, // #   #
	0x70, //  ### 

	// @10 '1' (3 pixels wide)
	0x20, //   #
	0x20, //   #
	0x20, //   #
	0x20, //   #
	0x20, //   #
	0x20, //   #
	0x20, //   #
	0x20, //   #
	0xE0, // ###
	0x20, //   #

	// @20 '2' (5 pixels wide)
	0xF8, // #####
	0x80, // #    
	0x80, // #    
	0x40, //  #   
	0x20, //   #  
	0x10, //    # 
	0x08, //     #
	0x88, // #   #
	0x88, // #   #
	0x70, //  ### 

	// @30 '3' (5 pixels wide)
	0x70, //  ### 
	0x88, // #   #
	0x08, //     #
	0x08, //     #
	0x08, //     #
	0x30, //   ## 
	0x08, //     #
	0x08, //     #
	0x88, // #   #
	0x70, //  ### 

	// @40 '4' (5 pixels wide)
	0x08, //     #
	0x08, //     #
	0x08, //     #
	0xF8, // #####
	0x88, // #   #
	0x48, //  #  #
	0x28, //   # #
	0x28, //   # #
	0x18, //    ##
	0x08, //     #

	// @50 '5' (5 pixels wide)
	0x70, //  ### 
	0x88, // #   #
	0x08, //     #
	0x08, //     #
	0x88, // #   #
	0xF0, // #### 
	0x80, // #    
	0x80, // #    
	0x80, // #    
	0xF8, // #####

	// @60 '6' (5 pixels wide)
	0x70, //  ### 
	0x88, // #   #
	0x88, // #   #
	0x88, // #   #
	0xC8, // ##  #
	0xB0, // # ## 
	0x80, // #    
	0x80, // #    
	0x88, // #   #
	0x70, //  ### 

	// @70 '7' (5 pixels wide)
	0x40, //  #   
	0x40, //  #   
	0x40, //  #   
	0x20, //   #  
	0x20, //   #  
	0x10, //    # 
	0x10, //    # 
	0x08, //     #
	0x08, //     #
	0xF8, // #####

	// @80 '8' (5 pixels wide)
	0x70, //  ### 
	0x88, // #   #
	0x88, // #   #
	0x88, // #   #
	0x88, // #   #
	0x70, //  ### 
	0x88, // #   #
	0x88, // #   #
	0x88, // #   #
	0x70, //  ### 

	// @90 '9' (5 pixels wide)
	0x70, //  ### 
	0x88, // #   #
	0x08, //     #
	0x08, //     #
	0x68, //  ## #
	0x98, // #  ##
	0x88, // #   #
	0x88, // #   #
	0x88, // #   #
	0x70, //  ### 
};

// Character descriptors for Microsoft Sans Serif 10pt
// { [Char width in bits], [Offset into microsoftSansSerif_10ptCharBitmaps in bytes] }
const FONT_CHAR_INFO microsoftSansSerif_8ptDescriptors[] =
{
	{5, 0}, 		// 0 
	{3, 10}, 		// 1 
	{5, 20}, 		// 2 
	{5, 30}, 		// 3 
	{5, 40}, 		// 4 
	{5, 50}, 		// 5 
	{5, 60}, 		// 6 
	{5, 70}, 		// 7 
	{5, 80}, 		// 8 
	{5, 90}, 		// 9 
};



// Font information for Microsoft Sans Serif 10pt
const FONT_INFO microsoftSansSerif_8ptFontInfo =
{
		10,		//  Character height in bits
		2, 		//  Character height
		'0', 	//  Start character
		'9', 	//  End character
		2,    	//  Width, in pixels, of space character
		microsoftSansSerif_8ptDescriptors, //  Character descriptor array
		microsoftSansSerif_8ptBitmaps, //  Character bitmap array
};



