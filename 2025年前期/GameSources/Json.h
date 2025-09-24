/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	enum class JsonType {
		Number, String, Bool, Object, Array, Null, Unknown
	};

	class JsonValue {
	protected:
		void Indent(int indent,wstring& str) {
			for (int i = 0; i < indent; i++) {
				str += L"\t";
			}
		}
	public:
		virtual ~JsonValue() {}
		virtual JsonType GetType()const { return JsonType::Unknown; }
		virtual wstring ToString(int indent = 0) { return L""; }

		bool CompareType(JsonType type) const {
			return GetType() == type;
		}

		/// <summary>
		/// shared_ptrに変換。スライス対策
		/// </summary>
		/// <returns>shared_ptr</returns>
		virtual shared_ptr<JsonValue> Clone()const {
			return make_shared<JsonValue>(*this);
		}
	};

	class JsonNumber : public JsonValue {
		float m_Value;
	public:
		JsonNumber(float value) :m_Value(value) {}
		virtual JsonType GetType() const override { return JsonType::Number; }
		virtual shared_ptr<JsonValue> Clone()const override {
			return make_shared<JsonNumber>(*this);
		}
		virtual wstring ToString(int indent = 0)override {
			return to_wstring(m_Value);
		}

		float GetFloatValue() const { return m_Value; }
		int GetIntValue() const { return static_cast<int>(m_Value); }
	};

	class JsonString : public JsonValue {
		wstring m_Value;
	public:
		JsonString(const wstring& value) :m_Value(value) {}
		virtual JsonType GetType() const override { return JsonType::String; }
		virtual shared_ptr<JsonValue> Clone()const override {
			return make_shared<JsonString>(*this);
		}
		virtual wstring ToString(int indent = 0) override {
			return L"\"" + m_Value + L"\"";
		}

		const wstring& GetValue() const { return m_Value; }
	};

	class JsonBool : public JsonValue {
		bool m_Value;
	public:
		JsonBool(bool value) :m_Value(value) {}
		virtual JsonType GetType() const override { return JsonType::Bool; }
		virtual shared_ptr<JsonValue> Clone()const override {
			return make_shared<JsonBool>(*this);
		}
		virtual wstring ToString(int indent = 0) override {
			return m_Value ? L"true" : L"false";
		}

		bool GetValue() const { return m_Value; }
	};
	class JsonNull : public JsonValue {
		public:
		JsonNull() {}
		virtual JsonType GetType() const override { return JsonType::Null; }
	};
	
	class JsonObject;
	class JsonArray : public JsonValue {
		JsonType m_ArrayType;
	public:
		vector<shared_ptr<JsonValue>> m_Values;

		JsonArray() {}
    //---------配列の初期化----------
	//    std::variant使いたい

		JsonArray(const vector<int>& array);
		JsonArray(const vector<float>& array);
		JsonArray(const vector<bool>& array);
		JsonArray(const vector<wstring>& array);
		JsonArray(const vector<JsonObject>& array);

		virtual JsonType GetType() const override { return JsonType::Array; }
		virtual shared_ptr<JsonValue> Clone()const override {
			return make_shared<JsonArray>(*this);
		}
		virtual wstring ToString(int indent = 0)override {
			wstring str = L"[";
			for (auto it = m_Values.begin(); it != m_Values.end(); it++) {
				auto value = *it;
				str += value->ToString(indent);

				if (next(it) != m_Values.end()) {
					str += L",";
				}
			}
			str += L"]";

			return str;
		}
		vector<int> GetIntArray();
		vector<float> GetFloatArray();
		vector<bool> GetBoolArray();
		vector<wstring> GetStringArray();
		vector<shared_ptr<JsonObject>> GetObjectArray();

		void Add(const shared_ptr<JsonValue>& value) {
			if (!m_Values.empty()) {
				if (!value->CompareType(m_Values[0]->GetType())) {
					throw BaseException(L"異なる型を配列に含めることはできません",L"", L"JsonArray::Add(const shared_ptr<JsonValue>&)");
				}
			}
			m_Values.push_back(value);
		}
	};
	struct ObjectData;

	class JsonObject : public JsonValue {
		map<wstring, shared_ptr<JsonValue>> m_Values;

	public:
		JsonObject() {}
		JsonObject(initializer_list<ObjectData> list);
		virtual JsonType GetType() const override { return JsonType::Object; }
		virtual shared_ptr<JsonValue> Clone()const override {
			return make_shared<JsonObject>(*this);
		}
		virtual wstring ToString(int indent = 0)override {
			wstring str = L"{\n";

			for (auto it = m_Values.begin(); it != m_Values.end(); it++) {
				Indent(indent, str);
				auto value = *it;
				str += L"\"" + value.first + L"\" : " + value.second->ToString(indent + 1);
				if (next(it) != m_Values.end()) {
					str += L",\n";
				}
			}
			str += L"\n";
			Indent(indent - 1, str);
			str += L"}";

			return str;
		}

		bool HasKey(const wstring& key) const {
			return m_Values.find(key) != m_Values.end();
		}

		inline shared_ptr<JsonValue> At(const wstring& key) {
			auto it = m_Values.find(key);
			if (it != m_Values.end()) {
				return it->second;
			}
			else {
				throw BaseException(L"キーが存在しません", key, L"JsonObject::At(const wstring&)");
			}
		}
		template<class T>
		inline shared_ptr<T> At(const wstring& key) {
			auto value = At(key);
			return dynamic_pointer_cast<T>(value);
		}
		inline void Set(const wstring& key, const shared_ptr<JsonValue>& value) {
			m_Values[key] = value;
		}

		inline shared_ptr<JsonValue> Add(const wstring& key, const JsonValue& value) {
			return Add(key, value.Clone());
		}
		inline shared_ptr<JsonValue> Add(const wstring& key, const shared_ptr<JsonValue>& value) {
			if (HasKey(key)) {
				throw BaseException(L"キーがすでに存在します", key, L"JsonObject::Add(const wstring&, const shared_ptr<JsonValue>&)");
			}
			m_Values[key] = value;

			return value;
		}
	};
//---------データとファイルデータの変換をするクラス----------
//       Jsonクラス内で使用される
	class JsonHelper {
	//----------数値変換----------
		shared_ptr<JsonValue> ParseValue(const wstring& str, size_t& pos);
		shared_ptr<JsonNumber> ParseNumber(const wstring& str, size_t& pos);
		shared_ptr<JsonBool> ParseBool(const wstring& str, size_t& pos);
		shared_ptr<JsonString> ParseString(const wstring& str, size_t& pos);
		shared_ptr<JsonArray> ParseArray(const wstring& str, size_t& pos);
		shared_ptr<JsonObject> ParseObject(const wstring& str, size_t& pos);

	//----------文字列操作----------
		void SkipSpace(const wstring& str, size_t& pos) {
			while (isspace(Peek(str, pos))) Get(str, pos);
		}
		wchar_t Peek(const wstring& str, size_t& pos) {
			if (pos >= str.size()) return L'\0';
			return str[pos];
		}
		wchar_t Get(const wstring& str, size_t& pos) {
			if (pos >= str.size()) return L'\0';
			return str[pos++];
		}
	public:
		/// <summary>
		/// JsonObject型のデータをファイルに保存する
		/// </summary>
		/// <param name="filename">保存先</param>
		/// <param name="data">データ</param>
		void ParseFile(const wstring& filename,const shared_ptr<JsonObject>& data);

		/// <summary>
		/// ファイルを読み込んでJsonObject型のデータに変換する
		/// </summary>
		/// <param name="filename">参照先</param>
		/// <returns>データ</returns>
		shared_ptr<JsonObject> ParseValue(const wstring& filename);

		/// <summary>
		/// パスをキーに変換する
		/// </summary>
		/// <param name="root">ルートパス</param>
		/// <returns>キー</returns>
		vector<wstring> SplitKey(const wstring& root);

		/// <summary>
		/// パスをたどる。ない場合は新しく作成される
		/// </summary>
		/// <param name="keys">キーの配列</param>
		/// <returns>たどった結果</returns>
		shared_ptr<JsonObject> EnsurePath(const vector<wstring>& keys,shared_ptr<JsonObject>& root);
	};

//---------Jsonのデータ管理を行うクラス----------
//       実際に使う際はこのクラスを宣言する
	class Json {
		shared_ptr<JsonObject> m_Root;
		JsonHelper m_Helper;
	public:
		Json() :m_Root(make_shared<JsonObject>()) {
			m_Helper = JsonHelper();
		}
		Json(const wstring& filename) {
			m_Helper = JsonHelper();
			Load(filename);
		}

		shared_ptr<JsonObject> GetRoot() const { return m_Root; }

		void Load(const wstring& filename) {
			m_Root = m_Helper.ParseValue(filename);
		}

		void Save(const wstring& filename) {
			m_Helper.ParseFile(filename, m_Root);
		}

		void Set(const wstring& root, const shared_ptr<JsonValue>& value);
		void Set(const wstring& root, const JsonValue& value) {
			Set(root, value.Clone());
		}
		shared_ptr<JsonValue> At(const wstring& root);

		template <class T>
		shared_ptr<T> At(const wstring& root) {
			return dynamic_pointer_cast<T>(At(root));
		}
	};
}
//end basecross
