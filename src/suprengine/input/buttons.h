#pragma once

#include <suprengine/utils/enum-flags.h>
#include <suprengine/utils/usings.h>

namespace suprengine
{
	/**
	 * Enum of all keys on the US English QWERTY keyboard layout, mapping correctly
	 * to any keyboard layout using the physical location of the keys.
	 * https://wiki.libsdl.org/SDL3/BestKeyboardPractices
	 */
	enum class PhysicalKey : uint16
	{
		None = 0,

		A = 4,
	    B = 5,
	    C = 6,
	    D = 7,
	    E = 8,
	    F = 9,
	    G = 10,
	    H = 11,
	    I = 12,
	    J = 13,
	    K = 14,
	    L = 15,
	    M = 16,
	    N = 17,
	    O = 18,
	    P = 19,
	    Q = 20,
	    R = 21,
	    S = 22,
	    T = 23,
	    U = 24,
	    V = 25,
	    W = 26,
	    X = 27,
	    Y = 28,
	    Z = 29,

	    One   = 30,
	    Two   = 31,
	    Three = 32,
	    Four  = 33,
	    Five  = 34,
	    Six   = 35,
	    Seven = 36,
	    Eight = 37,
	    Nine  = 38,
	    Zero  = 39,

	    Return = 40,
	    Escape = 41,
	    Backspace = 42,
	    Tab = 43,
	    Space = 44,

	    Minus = 45,
	    Equals = 46,
	    Leftbracket = 47,
	    Rightbracket = 48,
	    Backslash = 49, /**< Located at the lower left of the return
	                                  *   key on ISO keyboards and at the right end
	                                  *   of the QWERTY row on ANSI keyboards.
	                                  *   Produces REVERSE SOLIDUS (backslash) and
	                                  *   VERTICAL LINE in a US layout, REVERSE
	                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
	                                  *   layout, NUMBER SIGN and TILDE in a UK
	                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
	                                  *   in a Swiss German layout, NUMBER SIGN and
	                                  *   APOSTROPHE in a German layout, GRAVE
	                                  *   ACCENT and POUND SIGN in a French Mac
	                                  *   layout, and ASTERISK and MICRO SIGN in a
	                                  *   French Windows layout.
	                                  */
	    Nonushash = 50, /**< ISO USB keyboards actually use this code
	                                  *   instead of 49 for the same key, but all
	                                  *   OSes I've seen treat the two codes
	                                  *   identically. So, as an implementor, unless
	                                  *   your keyboard generates both of those
	                                  *   codes and your OS treats them differently,
	                                  *   you should generate BACKSLASH
	                                  *   instead of this code. As a user, you
	                                  *   should not rely on this code because SDL
	                                  *   will never generate it with most (all?)
	                                  *   keyboards.
	                                  */
	    Semicolon = 51,
	    Apostrophe = 52,
	    Grave = 53, /**< Located in the top left corner (on both ANSI
	                 *   and ISO keyboards). Produces GRAVE ACCENT and
	                 *   TILDE in a US Windows layout and in US and UK
	                 *   Mac layouts on ANSI keyboards, GRAVE ACCENT
	                 *   and NOT SIGN in a UK Windows layout, SECTION
	                 *   SIGN and PLUS-MINUS SIGN in US and UK Mac
	                 *   layouts on ISO keyboards, SECTION SIGN and
	                 *   DEGREE SIGN in a Swiss German layout (Mac:
	                 *   only on ISO keyboards), CIRCUMFLEX ACCENT and
	                 *   DEGREE SIGN in a German layout (Mac: only on
	                 *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
	                 *   French Windows layout, COMMERCIAL AT and
	                 *   NUMBER SIGN in a French Mac layout on ISO
	                 *   keyboards, and LESS-THAN SIGN and GREATER-THAN
	                 *   SIGN in a Swiss German, German, or French Mac
	                 *   layout on ANSI keyboards.
	                 */
	    Comma = 54,
	    Period = 55,
	    Slash = 56,

	    Capslock = 57,

	    F1 = 58,
	    F2 = 59,
	    F3 = 60,
	    F4 = 61,
	    F5 = 62,
	    F6 = 63,
	    F7 = 64,
	    F8 = 65,
	    F9 = 66,
	    F10 = 67,
	    F11 = 68,
	    F12 = 69,

	    PrintScreen = 70,
	    ScrollLock = 71,
	    Pause = 72,
	    Insert = 73, /**< insert on PC, help on some Mac keyboards (but
	                                   does send code 73, not 117) */
	    Home = 74,
	    PageUp = 75,
	    Delete = 76,
	    End = 77,
	    PageDown = 78,
	    Right = 79,
	    Left = 80,
	    Down = 81,
	    Up = 82,

	    NumLockClear = 83, /**< num lock on PC, clear on Mac keyboards */
	    NumericDivide = 84,
	    NumericMultiply = 85,
	    NumericMinus = 86,
	    NumericPlus = 87,
	    NumericEnter = 88,
	    Numeric1 = 89,
	    Numeric2 = 90,
	    Numeric3 = 91,
	    Numeric4 = 92,
	    Numeric5 = 93,
	    Numeric6 = 94,
	    Numeric7 = 95,
	    Numeric8 = 96,
	    Numeric9 = 97,
	    Numeric0 = 98,
	    NumericPeriod = 99,

	    NonUSBackslash = 100, /**< This is the additional key that ISO
	                                        *   keyboards have over ANSI ones,
	                                        *   located between left shift and Y.
	                                        *   Produces GRAVE ACCENT and TILDE in a
	                                        *   US or UK Mac layout, REVERSE SOLIDUS
	                                        *   (backslash) and VERTICAL LINE in a
	                                        *   US or UK Windows layout, and
	                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
	                                        *   in a Swiss German, German, or French
	                                        *   layout. */
	    Application = 101, /**< windows contextual menu, compose */
	    Power = 102, /**< The USB document says this is a status flag,
	                               *   not a physical key - but some Mac keyboards
	                               *   do have a power key. */
	    NumericEquals = 103,
	    F13 = 104,
	    F14 = 105,
	    F15 = 106,
	    F16 = 107,
	    F17 = 108,
	    F18 = 109,
	    F19 = 110,
	    F20 = 111,
	    F21 = 112,
	    F22 = 113,
	    F23 = 114,
	    F24 = 115,
	    Execute = 116,
	    Help = 117,
	    Menu = 118,
	    Select = 119,
	    Stop = 120,
	    Again = 121,   /**< redo */
	    Undo = 122,
	    Cut = 123,
	    Copy = 124,
	    Paste = 125,
	    Find = 126,
	    Mute = 127,
	    VolumeUp = 128,
	    VolumeDown = 129,
	/* not sure whether there's a reason to enable these */
	/*     Lockingcapslock = 130,  */
	/*     Lockingnumlock = 131, */
	/*     Lockingscrolllock = 132, */
	    NumericComma = 133,
	    NumericEqualSas400 = 134,

	    International1 = 135, /**< used on Asian keyboards, see
	                                            footnotes in USB doc */
	    International2 = 136,
	    International3 = 137, /**< Yen */
	    International4 = 138,
	    International5 = 139,
	    International6 = 140,
	    International7 = 141,
	    International8 = 142,
	    International9 = 143,
	    Lang1 = 144, /**< Hangul/English toggle */
	    Lang2 = 145, /**< Hanja conversion */
	    Lang3 = 146, /**< Katakana */
	    Lang4 = 147, /**< Hiragana */
	    Lang5 = 148, /**< Zenkaku/Hankaku */
	    Lang6 = 149, /**< reserved */
	    Lang7 = 150, /**< reserved */
	    Lang8 = 151, /**< reserved */
	    Lang9 = 152, /**< reserved */

	    AltErase = 153, /**< Erase-Eaze */
	    Sysreq = 154,
	    Cancel = 155,
	    Clear = 156,
	    Prior = 157,
	    Return2 = 158,
	    Separator = 159,
	    Out = 160,
	    Oper = 161,
	    ClearAgain = 162,
	    Crsel = 163,
	    Exsel = 164,

	    Numeric00 = 176,
	    Numeric000 = 177,
	    ThousandsSeparator = 178,
	    DecimalSeparator = 179,
	    CurrencyUnit = 180,
	    CurrencySubUnit = 181,
	    NumericLeftParenthesis = 182,
	    NumericRightParenthesis = 183,
	    NumericLeftBrace = 184,
	    NumericRightBrace = 185,
	    NumericTab = 186,
	    NumericBackspace = 187,
	    NumericA = 188,
	    NumericB = 189,
	    NumericC = 190,
	    NumericD = 191,
	    NumericE = 192,
	    NumericF = 193,
	    NumericXor = 194,
	    NumericPower = 195,
	    NumericPercent = 196,
	    NumericLess = 197,
	    NumericGreater = 198,
	    NumericAmpersand = 199,
	    NumericDblAmpersand = 200,
	    NumericVerticalBar = 201,
	    NumericDblVerticalBar = 202,
	    NumericColon = 203,
	    NumericHash = 204,
	    NumericSpace = 205,
	    NumericAt = 206,
	    NumericExclam = 207,
	    NumericMemStore = 208,
	    NumericMemRecall = 209,
	    NumericMemClear = 210,
	    NumericMemAdd = 211,
	    NumericMemSubtract = 212,
	    NumericMemMultiply = 213,
	    NumericMemDivide = 214,
	    NumericPlusMinus = 215,
	    NumericClear = 216,
	    NumericClearEntry = 217,
	    NumericBinary = 218,
	    NumericOctal = 219,
	    NumericDecimal = 220,
	    NumericHexadecimal = 221,

	    LeftCtrl = 224,
	    LeftShift = 225,
	    LeftAlt = 226, /**< alt, option */
	    LeftGui = 227, /**< windows, command (apple), meta */
	    RightCtrl = 228,
	    RightShift = 229,
	    RightAlt = 230, /**< alt gr, option */
	    RightGui = 231, /**< windows, command (apple), meta */

	    Mode = 257,    /**< I'm not sure if this is really not covered
	                                 *   by any of the above, but since there's a
	                                 *   special KMOD_MODE for it I'm adding it here
	                                 */

	    /* @} *//* Usage page 0x07 */

	    /**
	     *  \name Usage page 0x0C
	     *
	     *  These values are mapped from usage page 0x0C (USB consumer page).
	     */
	    /* @{ */

	    AudionNext = 258,
	    AudioPrevious = 259,
	    AudioStop = 260,
	    AudioPlay = 261,
	    AudioMute = 262,
	    MediaSelect = 263,
	    Www = 264,
	    Mail = 265,
	    Calculator = 266,
	    Computer = 267,
	    AC_Search = 268,
	    AC_Home = 269,
	    AC_Back = 270,
	    AC_Forward = 271,
	    AC_Stop = 272,
	    AC_Refresh = 273,
	    AC_Bookmarks = 274,

	    /* @} *//* Usage page 0x0C */

	    /**
	     *  \name Walther keys
	     *
	     *  These are values that Christian Walther added (for mac keyboard?).
	     */
	    /* @{ */

	    BrightnessDown = 275,
	    BrightnessUp = 276,
	    DisplaySwitch = 277, /**< display mirroring/dual display
	                                           switch, video mode switch */
	    Kbdillumtoggle = 278,
	    Kbdillumdown = 279,
	    Kbdillumup = 280,
	    Eject = 281,
	    Sleep = 282,

	    App1 = 283,
	    App2 = 284,

	    AudioRewind = 285,
	    AudioFastForward = 286,
	};

	/**
	 * Enum of all keys on the current keyboard layout.
	 * https://wiki.libsdl.org/SDL3/BestKeyboardPractices
	 */
	enum class VirtualKey : uint16
	{
		// TODO
	};

	enum class KeyState : uint8
	{
		Up,
		Down,
		Pressed,
		Released,
	};

	enum class MouseButton : uint8
	{
		None   = 0,
		Left   = 1 << 0,
		Middle = 1 << 1,
		Right  = 1 << 2,
	};
	DEFINE_ENUM_WITH_FLAGS( MouseButton, uint8 )

	/*
	 * Enum flags, holding all generic gamepad buttons.
	 * Values are purposely mapped to XInput buttons values for a better integration.
	 */
	enum class GamepadButton : uint16
	{
		None			= 0,
		DpadUp			= 1 << 0,
		DpadDown		= 1 << 1,
		DpadLeft		= 1 << 2,
		DpadRight		= 1 << 3,
		Start			= 1 << 4,
		Back			= 1 << 5,
		LeftThumb		= 1 << 6,
		RightThumb		= 1 << 7,
		LeftShoulder	= 1 << 8,
		RightShoulder	= 1 << 9,

		// XInput does not implement the trigger values as buttons.
		// Fortunately, the mapping of further values is still correct because it jumped over them.

		LeftTrigger		= 1 << 10,
		RightTrigger	= 1 << 11,

		FaceButtonDown	= 1 << 12,
		FaceButtonRight = 1 << 13,
		FaceButtonLeft	= 1 << 14,
		FaceButtonUp	= 1 << 15,
	};
	DEFINE_ENUM_WITH_FLAGS( GamepadButton, uint16 )

	enum class JoystickSide
	{
		None,
		Left,
		Right,
	};

	static MouseButton sdl_to_mouse_button( const uint8 button_index )
	{
		switch ( button_index )
		{
			case 1:
				return MouseButton::Left;
			case 2:
				return MouseButton::Middle;
			case 3:
				return MouseButton::Right;
		}

		return MouseButton::None;
	}
}