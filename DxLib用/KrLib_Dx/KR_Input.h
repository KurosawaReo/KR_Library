/*
   - KR_Input.h - (DxLib)
   ver.2026/04/16

   入力操作機能。
   (オブジェクト指向ver → KR_Object)
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_DX_GLOBAL
  #include "KR_Global.h"
#endif
//[include] hで使うもの.
#include "KR_ManagerBase.h"

//KrLib名前空間.
namespace KR
{
	//キーボードID.
	enum class KeyID
	{
		Down   = KEY_INPUT_DOWN,
		Left   = KEY_INPUT_LEFT,
		Right  = KEY_INPUT_RIGHT,
		Up     = KEY_INPUT_UP,
		Num0   = KEY_INPUT_0,
		Num1   = KEY_INPUT_1,
		Num2   = KEY_INPUT_2,
		Num3   = KEY_INPUT_3,
		Num4   = KEY_INPUT_4,
		Num5   = KEY_INPUT_5,
		Num6   = KEY_INPUT_6,
		Num7   = KEY_INPUT_7,
		Num8   = KEY_INPUT_8,
		Num9   = KEY_INPUT_9,
		A      = KEY_INPUT_A,
		B      = KEY_INPUT_B,
		C      = KEY_INPUT_C,
		D      = KEY_INPUT_D,
		E      = KEY_INPUT_E,
		F      = KEY_INPUT_F,
		G      = KEY_INPUT_G,
		H      = KEY_INPUT_H,
		I      = KEY_INPUT_I,
		J      = KEY_INPUT_J,
		K      = KEY_INPUT_K,
		L      = KEY_INPUT_L,
		M      = KEY_INPUT_M,
		N      = KEY_INPUT_N,
		O      = KEY_INPUT_O,
		P      = KEY_INPUT_P,
		Q      = KEY_INPUT_Q,
		R      = KEY_INPUT_R,
		S      = KEY_INPUT_S,
		T      = KEY_INPUT_T,
		U      = KEY_INPUT_U,
		V      = KEY_INPUT_V,
		W      = KEY_INPUT_W,
		X      = KEY_INPUT_X,
		Y      = KEY_INPUT_Y,
		Z      = KEY_INPUT_Z,
		Esc    = KEY_INPUT_ESCAPE,   //ESCキー.
		ShiftL = KEY_INPUT_LSHIFT,   //SHIFTキー(左)
		ShiftR = KEY_INPUT_RSHIFT,   //SHIFTキー(右)
		CtrlL  = KEY_INPUT_LCONTROL, //CTRLキー(左)
		CtrlR  = KEY_INPUT_RCONTROL, //CTRLキー(右)
		Space  = KEY_INPUT_SPACE,
		Enter  = KEY_INPUT_RETURN,   //ENTERキー.
	};
	//マウスID.
	enum class MouseID
	{
		Left,   //0x0001
		Right,  //0x0002
		Middle, //0x0004
	};
	//コントローラID(xbox基準)
	enum class PadXboxID
	{
		Down,	//0x0001
		Left,	//0x0002
		Right,	//0x0004
		Up,		//0x0008
		A,		//0x0010: 下のボタン.
		B,		//0x0020: 右のボタン.
		X,		//0x0040: 左のボタン.
		Y,		//0x0080: 上のボタン.
		L,		//0x0100
		R,		//0x0200
		View,	//0x0400
		Menu,	//0x0800
		StickL, //0x1000: 左スティック押し込み.
		StickR, //0x2000: 右スティック押し込み.
	};
	//コントローラID(switch基準)
	enum class PadSwitchID
	{
		Down,	 //0x00001
		Left,	 //0x00002
		Right,	 //0x00004
		Up,		 //0x00008
		B,		 //0x00010: 下のボタン.
		A,		 //0x00020: 右のボタン.
		Y,		 //0x00040: 左のボタン.
		X,		 //0x00080: 上のボタン.
		L,		 //0x00100
		R,		 //0x00200
		ZL,		 //0x00400
		ZR,		 //0x00800
		Minus,	 //0x01000: -ボタン.
		Plus,	 //0x02000: +ボタン.
		StickL,	 //0x04000: 左スティック押し込み.
		StickR,	 //0x08000: 右スティック押し込み.
		Home,	 //0x10000: ホームボタン.
		Capture, //0x20000: キャプチャーボタン.
	};
	//コントローラID(アーケード筐体)
	enum class PadArcadeID
	{
		Down,
		Left,
	    Right,
		Up,
		BtnUpper1,
		BtnLower1,
		BtnLower2,
		BtnUpper2,
		Unknown1,  //不明.
		BtnUpper3,
		Unknown2,  //不明.
		BtnLower3,
		Unknown3,  //不明.
		BtnStart,
	};
	//何の操作か(Action登録用)
	enum class InputType
	{
		Key,
		Mouse,
		PadXbox,
		PadSwitch,
		PadArcade
	};

	//入力データ(単体)
	struct InputData
	{
		InputType type; //何の操作か.
		int       id;   //操作ID.
	};
	//アクションデータ.
	struct ActionData
	{
		vector<InputData> inputs; //登録する入力データ.
		int time;                 //入力時間.
	};

	//キーやボタンの種類の最大数(変更禁止)
	constexpr int KEY_MAX     = 256;
	constexpr int MOUSE_MAX   = 3;
	constexpr int PAD_BTN_MAX = 32;

	//入力管理クラス.
	class InputMng final : public ManagerBase
	{
	//▼ ===== 実体 ===== ▼.
	private:
		static InputMng inst; //実体を入れる用.

	//▼ ===== 変数 ===== ▼.
	private:
		int tmKey   [KEY_MAX]{};     //キーを押している時間.
		int tmMouse [MOUSE_MAX]{};   //マウスを押下している時間.            (index: bitフラグ)
		int tmPadBtn[PAD_BTN_MAX]{}; //コントローラボタンを押下している時間.(index: bitフラグ)

		INT_XY mPos{};     //マウス座標.
		INT_XY stickVec{}; //スティック入力.

		umap<string, ActionData> actions{}; //アクション記録用.

	//▼ ===== 関数 ===== ▼.
	private:
		//コンストラクタ.
		InputMng() : ManagerBase(ORDER_KR_INPUT_MNG) {}

		DBL_XY GetVector4Dir(INT_XY pow);
		void   UpdateKey();
		void   UpdateMouse();
		void   UpdatePad();
		void   UpdateAction();

	public:

		//操作判定.
		static bool   IsPushKey       (KeyID id);
		static int    IsPushKeyTime   (KeyID id);
		static bool   IsPushMouse     (MouseID id);
		static int    IsPushMouseTime (MouseID id);
		static bool   IsPushPadBtn    (PadXboxID   id);
		static bool   IsPushPadBtn    (PadSwitchID id);
		static bool   IsPushPadBtn    (PadArcadeID id);
		static int    IsPushPadBtnTime(PadXboxID   id);
		static int    IsPushPadBtnTime(PadSwitchID id);
		static int    IsPushPadBtnTime(PadArcadeID id);
		static bool   IsPushAction    (string name);
		static int    IsPushActionTime(string name);

		//アクション.
		static void   AddAction       (string name, KeyID       id);
		static void   AddAction       (string name, MouseID     id);
		static void   AddAction       (string name, PadXboxID   id);
		static void   AddAction       (string name, PadSwitchID id);
		static void   AddAction       (string name, PadArcadeID id);

		//取得.
		static DBL_XY GetKey4Dir();
		static DBL_XY GetPad4Dir();
		static DBL_XY GetPadStick();
		static DBL_XY GetMousePos();

		//基本処理.
		void Init()   override {} //未使用.
		void Reset()  override {} //未使用.
		void Update() override;
		void Draw()   override {} //未使用.

		//使用禁止.
		InputMng(const InputMng&) = delete;
		InputMng& operator=(const InputMng&) = delete;
	};
}
