#ifndef __MARQUEE_INSERT_UPDATE_H__
#define __MARQUEE_INSERT_UPDATE_H__
class MarqueeInsertUpdateRequest
{
	friend class MarqueeInsertUpdateResponse;
public:
	MarqueeInsertUpdateRequest(){};
	MarqueeInsertUpdateRequest(const string& json) {
		this->Deserialize(json);
	}
	template <typename Writer>
	void Serialize(Writer& writer) const {
		writer.StartObject();
		SERIALIZE_MEMBER(writer,id);
		SERIALIZE_MEMBER(writer,title);
		SERIALIZE_MEMBER(writer,type);
		SERIALIZE_MEMBER(writer,area);
		SERIALIZE_MEMBER(writer,content);
		SERIALIZE_MEMBER(writer,beginTime);
		SERIALIZE_MEMBER(writer,endTime);
		SERIALIZE_MEMBER(writer,interval);
		SERIALIZE_MEMBER(writer,count);
		SERIALIZE_MEMBER(writer,status);
		SERIALIZE_MEMBER(writer,optUser);
		SERIALIZE_MEMBER(writer,createDate);

		writer.EndObject();
	}

	void toString(std::string& json) const {
		StringBuffer sb;
		Writer<StringBuffer> writer(sb);
		Serialize(writer);
		json = sb.GetString();
	}

	//转换时间戳
	time_t GetTimeStamp(string time)
	{
	    struct tm timeinfo;
	    strptime(time.c_str(), "%Y-%m-%d %H:%M:%S",  &timeinfo);
	    time_t timeStamp = mktime(&timeinfo);
	    return timeStamp;
	}

	void Deserialize(const string& json)
	{
		Document d;
		if (d.Parse(json.c_str()).HasParseError()){
			throw logic_error("parse json error. raw data : " + json);
		}
		SET_DOC_MEMBER(d,id);
		SET_DOC_MEMBER(d,title);
		SET_DOC_MEMBER(d,type);
		SET_DOC_MEMBER(d,area);
		SET_DOC_MEMBER(d,content);
		SET_DOC_MEMBER(d,beginTime);
		SET_DOC_MEMBER(d,endTime);
		SET_DOC_MEMBER(d,interval);
		SET_DOC_MEMBER(d,count);
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
	CString			_title 			;  //标题
	CInteger        _type           ;  //0 广播  1 停服
	CString        	_area           ;  //发送消息的地区：台湾、东南亚、欧洲、大陆等。
	CString        	_content        ;  //跑马灯内容
	CString        	_beginTime      ;  //开始播放时间
	CString        	_endTime        ;  //结束播放时间
	CInteger       	_interval       ;  //间隔时间（s）
	CInteger 		_count 			;  //循环次数
	CInteger       	_status         ;  //状态 (0：关闭  1：开启)
	CString        	_optUser        ;  //操作者
	CString        	_createDate     ;  //创建时间

};
class MarqueeInsertUpdateResponse
{
public:
	MarqueeInsertUpdateResponse(const string& json) {
		this->Deserialize(json);
	}
	template <typename Writer>
	void Serialize(Writer& writer) const {
		writer.StartObject();

		writer.EndObject();
	}

	void toString(std::string& json) const {
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
	}

	static tars::Int32 handler(const vector<tars::Char>& reqBuf, const map<std::string, std::string>& extraInfo, vector<tars::Char>& rspBuf)
	{
		__TRY__
        // STEP1 解码
        MarqueeInsertUpdateRequest request;
        decode(reqBuf, request);

        // STEP2 具体业务处理
        int64_t resultCode = RESULT_CODE_SUCCESS;

        push::BroadcastNotify notify;
        notify.content = request._content.toString();

        if(request._type == 0)
        {
			notify.type = 0;//客服广播
			notify.param.interval = request._interval;
			notify.param.count = request._count;
        }
        else
        {
        	notify.type = 1;//停服公告
			notify.param.beginTime = request.GetTimeStamp(request._beginTime.toString());
			notify.param.endTime = request.GetTimeStamp(request._endTime.toString());

			LOG_DEBUG << "beginTime:"<< notify.param.beginTime << ", endTime:"<< notify.param.endTime << endl;

			//通知系统消息
        	g_app.getOuterFactoryPtr()->getGlobalServantPrx(0)->async_serverUpdateMessage(NULL, notify.param.beginTime, notify.param.endTime, request._title, request._content);
        }

		g_app.getOuterFactoryPtr()->getPushServerPrx(0)->async_pushBroadcast(NULL, notify);

		// STEP3 填充数据
        encode(resultCode, request, rspBuf);

		__CATCH__
	    return 0;
	}
private:
    static void encode(int64_t resultCode, const MarqueeInsertUpdateRequest &request, vector<tars::Char> &rspBuf)
    {
		GMResponse rsp(resultCode, "", "", 0, 0);
		std::string resultJson;
		rsp.toString(resultJson);
		rspBuf.assign(resultJson.begin(), resultJson.end());
    }

};
#endif
