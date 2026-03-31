/*
   - KR_ManagerBase.h - (DxLib)
   ver.2026/02/19

   管理クラスの根底。

   [注意]
   App::InitDx()内でInit()が自動で呼ばれるため
   main関数が動く前に、ManagerBaseを継承した全クラスの実体を生成する必要がある。
   シングルトンにするのがおすすめ。
*/
#pragma once
//[include] KR_Global.
#if !defined DEF_KR_DX_GLOBAL
  #include "KR_Global.h"
#endif

//KrLib名前空間.
namespace KR
{
	//KrLibの管理クラスのorder値.
	constexpr int ORDER_KR_INPUT_MNG = -1;
	constexpr int ORDER_KR_SOUND_MNG = -1;

	//管理クラスの自動実行モード.
	enum class MngAutoExe
	{
		Active,		//Update & Draw
		UpdateOnly, //Updateのみ.
		DrawOnly,	//Drawのみ.
		Stop		//実行しない.
	};

	//前方宣言.
	class ManagerBase;

	//実体管理クラス.
	class ManagerInsts
	{
	//▼ ===== 実体 ===== ▼.
	public:
		static ManagerInsts& GetInst() {
			static ManagerInsts inst; //初呼び出し時に生成する.
			return inst;
		}

	//▼ ===== 変数 ===== ▼.
	private:
		vector<ManagerBase*> mngInsts; //インスタンス配列.

	//▼ ===== 関数 ===== ▼.
	private:
		//コンストラクタ.
		ManagerInsts(){}
		
		//管理クラスを探す.
		ManagerBase* GetMngClass(const std::type_info& type);

	public:
		//管理クラスを追加.
		void Push(ManagerBase* _inst);
		//管理クラスを取得.
		template<class T>
		T* Get() {
			return static_cast<T*>(GetMngClass(typeid(T)));
		}
		//管理クラスを全て取得.
		vector<ManagerBase*>& GetAll() { return mngInsts; }
		//order値で並べ替える.
		void SortOrder();

		//使用禁止.
		ManagerInsts(const ManagerInsts&) = delete;
		ManagerInsts& operator=(const ManagerInsts&) = delete;
	};

	/*
	   管理クラスの根底[継承想定]
	   
	   Init, Update, Drawは自動でAppクラスから呼び出される.
	   (order値が小さいほど先に実行)
	*/
	class ManagerBase
	{
	//▼ ===== 変数 ===== ▼.
	private: 
		MngAutoExe mode;    //自動実行モード.
		MngAutoExe befMode; //自動実行モード(1つ前保存用)
		int        order;   //処理優先度.

	//▼ ===== 関数 ===== ▼.
	public:
		//コンストラクタ.
		ManagerBase(int _order, MngAutoExe _mode = MngAutoExe::Active);
		//デストラクタ(これがあると安全?)
		virtual ~ManagerBase() = default;

		//order値.
		void       SetOrder(int _order);
		int        GetOrder() const { return order; }
		//自動実行モード.
		void       SetAutoExeMode(MngAutoExe _mode) { 
			befMode  = mode;    //元のモードを保存.
			mode     = _mode;   //モード切り替え.
		}
		void       BackAutoExeMode() {
			auto tmp = mode;
			mode     = befMode; //1つ前のモードに戻す.
			befMode  = tmp;     //元のモードを保存.
		}
		MngAutoExe GetAutoExeMode() const { return mode; }

		//判定.
		bool IsAutoUpdate() const {
			return mode == MngAutoExe::Active || mode == MngAutoExe::UpdateOnly;
		}
		bool IsAutoDraw() const {
			return mode == MngAutoExe::Active || mode == MngAutoExe::DrawOnly;
		}

		virtual void Init()   = 0;
		virtual void Reset()  = 0;
		virtual void Update() = 0;
		virtual void Draw()   = 0;
	};
}