#ifndef __PROPS_CONFIG_LIST_H__
#define __PROPS_CONFIG_LIST_H__
class PropsConfigListRequest
{
public:
	PropsConfigListRequest(const string& json) {
		this->Deserialize(json);
	}
	template <typename Writer>
	void Serialize(Writer& writer) const {
		writer.StartObject();
		SERIALIZE_MEMBER(writer,type);
		SERIALIZE_MEMBER(writer,startDate);
		SERIALIZE_MEMBER(writer,endDate);

		writer.EndObject();
	}

	void toString(std::string& json) {
		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		Serialize(writer);
		json = sb.GetString();
	}

	void Deserialize(const string& json)
	{
		Document d;
		if (d.Parse(json.c_str()).HasParseError()){
			throw logic_error("parse json error. raw data : " + json);
		}
		SET_DOC_MEMBER(d,type);
		SET_DOC_MEMBER(d,startDate);
		SET_DOC_MEMBER(d,endDate);

	}

	static tars::Int32 handler(const vector<tars::Char>& reqBuf, const map<std::string, std::string>& extraInfo, vector<tars::Char>& rspBuf)
	{
	    return 0;
	}
private:
	CInteger       	_type           ;  //道具类型 1=金币 2=钻石 3=门票道具 4=俱乐部货币 5=实物道具
	CString        	_startDate      ;  //开始时间
	CString        	_endDate        ;  //结束时间

};
class PropsConfigListResponse
{
public:
	PropsConfigListResponse(const string& json) {
		this->Deserialize(json);
	}
	template <typename Writer>
	void Serialize(Writer& writer) const {
		writer.StartObject();
		SERIALIZE_MEMBER(writer,id);
		SERIALIZE_MEMBER(writer,propsId);
		SERIALIZE_MEMBER(writer,type);
		SERIALIZE_MEMBER(writer,propsName);
		SERIALIZE_MEMBER(writer,icon);
		SERIALIZE_MEMBER(writer,ifTrade);
		SERIALIZE_MEMBER(writer,ifGive);
		SERIALIZE_MEMBER(writer,optUser);
		SERIALIZE_MEMBER(writer,createDate);

		writer.EndObject();
	}

	void toString(std::string& json) {
		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		Serialize(writer);
		json = sb.GetString();
	}

	void Deserialize(const string& json)
	{
		Document d;
		if (d.Parse(json.c_str()).HasParseError()){
			throw logic_error("parse json error. raw data : " + json);
		}
		SET_DOC_MEMBER(d,id);
		SET_DOC_MEMBER(d,propsId);
		SET_DOC_MEMBER(d,type);
		SET_DOC_MEMBER(d,propsName);
		SET_DOC_MEMBER(d,icon);
		SET_DOC_MEMBER(d,ifTrade);
		SET_DOC_MEMBER(d,ifGive);
		SET_DOC_MEMBER(d,optUser);
		SET_DOC_MEMBER(d,createDate);

	}

	static tars::Int32 handler(const vector<tars::Char>& reqBuf, const map<std::string, std::string>& extraInfo, vector<tars::Char>& rspBuf)
	{
	    return 0;
	}
private:
	CInteger       	_id             ;  //主键
	CInteger       	_propsId        ;  //道具Id
	CInteger       	_type           ;  //道具类型 1=金币 2=钻石 3=门票道具 4=俱乐部货币 5=实物道具
	CString        	_propsName      ;  //道具名称
	CString         _icon           ;  //图标
	CInteger       	_ifTrade        ;  //是否可交易 (0:否 1：是)
	CInteger       	_ifGive         ;  //是否可赠送 (0: 否 1：是)
	CString        	_optUser        ;  //操作者
	CString        	_createDate     ;  //创建时间

};
#endif