#ifndef __MARQUEE_LIST_H__
#define __MARQUEE_LIST_H__
class MarqueeListRequest
{
public:
	MarqueeListRequest(const string& json) {
		this->Deserialize(json);
	}
	template <typename Writer>
	void Serialize(Writer& writer) const {
		writer.StartObject();
		SERIALIZE_MEMBER(writer,title);
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
		SET_DOC_MEMBER(d,title);
		SET_DOC_MEMBER(d,startDate);
		SET_DOC_MEMBER(d,endDate);

	}

	static tars::Int32 handler(const vector<tars::Char>& reqBuf, const map<std::string, std::string>& extraInfo, vector<tars::Char>& rspBuf)
	{
	    return 0;
	}
private:
	CString        	_title          ;  //跑马灯标题
	CString        	_startDate      ;  //开始时间
	CString        	_endDate        ;  //结束时间

};
class MarqueeListResponse
{
public:
	MarqueeListResponse(const string& json) {
		this->Deserialize(json);
	}
	template <typename Writer>
	void Serialize(Writer& writer) const {
		writer.StartObject();
		SERIALIZE_MEMBER(writer,id);
		SERIALIZE_MEMBER(writer,title);
		SERIALIZE_MEMBER(writer,area);
		SERIALIZE_MEMBER(writer,content);
		SERIALIZE_MEMBER(writer,beginTime);
		SERIALIZE_MEMBER(writer,endTime);
		SERIALIZE_MEMBER(writer,time);
		SERIALIZE_MEMBER(writer,status);
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
		SET_DOC_MEMBER(d,title);
		SET_DOC_MEMBER(d,area);
		SET_DOC_MEMBER(d,content);
		SET_DOC_MEMBER(d,beginTime);
		SET_DOC_MEMBER(d,endTime);
		SET_DOC_MEMBER(d,time);
		SET_DOC_MEMBER(d,status);
		SET_DOC_MEMBER(d,optUser);
		SET_DOC_MEMBER(d,createDate);

	}

	static tars::Int32 handler(const vector<tars::Char>& reqBuf, const map<std::string, std::string>& extraInfo, vector<tars::Char>& rspBuf)
	{
	    return 0;
	}
private:
	CInteger       	_id             ;  //主键
	CString        	_title          ;  //跑马灯标题
	CString        	_area           ;  //发送消息的地区：台湾、东南亚、欧洲、大陆等。
	CString        	_content        ;  //跑马灯内容
	CString        	_beginTime      ;  //开始播放时间
	CString        	_endTime        ;  //结束播放时间
	CInteger       	_time           ;  //间隔时间（s）
	CInteger       	_status         ;  //状态 (0：关闭  1：开启)
	CString        	_optUser        ;  //操作者
	CString        	_createDate     ;  //创建时间

};
#endif