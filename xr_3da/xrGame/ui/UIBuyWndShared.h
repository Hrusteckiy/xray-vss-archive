#pragma once


//. #define _new_buy_wnd

#ifdef _new_buy_wnd
	class		CUIMpTradeWnd;
	typedef		CUIMpTradeWnd			BUY_WND_TYPE;
#else
	class		CUIBuyWnd;
	typedef		CUIBuyWnd				BUY_WND_TYPE;
#endif
