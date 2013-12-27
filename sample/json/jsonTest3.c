#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

#define LOGSIZE 3		
#define LOGVERSION "1.0"			///< 로그 버전 정의
#define LOGSYSTEM "GTS"				///< 로그 시스템 정의
#define LOGSUBSYSTEM "buf"		///< 로그 서브시스템 정의
#define LOGTYPE "app"					///< 로그 로그타입 정의
#define LOGREGKEY "none"			///< 로그 REGKEY 정의
#define LOGACTIONID "0"				///< 로그 ACTION ID 정의
#define LOGACTIONTYPE "enc"		///< 로그 ACTION TYPE 정의
#define LOGHOSTNAME "none"		///< 로그 호스트명 정의
#define LOGJOBID "none"				///< 로그 JOB ID 정의
#define LOGJOBELAPSED 0				///< 로그 작업수행시간 정의

typedef struct _HEADER
{  
  char strVersion[6];			///< 로그 버전 정보(1.00)
  char strDate[30];				///< 로그생성일자 및 시간
  char strSystem[20];			///< 시스템명("GTS")
  char strSubSystem[10];	///< 서브시스템명("buf" : BUFR)
  char strLogType[10];		///< 로그타입("app" : application)
  char strRegKey[50];			///< 특정자료에 대한 작업처리규칙 ("none" : 해당사항없음)
  
} HEADER;
typedef HEADER *PHEADER;


typedef struct _BODY
{	
	char strActionID[20];			///< Action ID(REGKEY에 정의된 개별액션 ("none" : 해당사항없음)
	char strActionType[5];		///< Action 형태( "enc" : encoding)
	char strActionApp[20];		///< 프로세스명
	char strHostname[20];			///< 동작한 호스트명	
	char strJobID[30];				///< 작업별 고유식별자 ("none" : 해당사항없음)
	int  nJobStatus;					///< 작업상태코드( 1:시작, 3:종료, 8:작업중단(오류) )
	int	 nJobElapsed;					///< 작업수행시간(ms) 
} BODY;
typedef BODY *PBODY;	


typedef struct _JSONLOG
{
	HEADER Header;			///< 로그체계 HEADER
	BODY Body;					///< 로그체계 BODY
} JSONLOG;
typedef JSONLOG *PJSONLOG;


JSONLOG gLog[LOGSIZE];		///< 로그구조체배열 전역변수
int gLogIndex = 0;	


void initalize()
{
	gLogIndex = 0;
	memset(gLog, 0x00, sizeof(JSONLOG) * LOGSIZE);
}

void finalize()
{
	//free(gLog);
}

int setHeaderData(char *pstrVersion, char *pstrDate, char *pstrSystem, char *pstrSubSystem, char *pstrLogType, char *pstrRegKey)
{
	if(pstrVersion == NULL || pstrDate == NULL || pstrSystem == NULL || pstrSubSystem == NULL || pstrLogType == NULL || pstrRegKey == NULL){
		fprintf(stderr, "Error : string parameters is null\t\t[File : %s, Line : %d, Function : %s]\n", __FILE__, __LINE__, __FUNCTION__);
		return -1;
	}
	strcpy(gLog[gLogIndex].Header.strVersion, pstrVersion);
	strcpy(gLog[gLogIndex].Header.strDate, pstrDate);
	strcpy(gLog[gLogIndex].Header.strSystem, pstrSystem);
	strcpy(gLog[gLogIndex].Header.strSubSystem, pstrSubSystem);
	strcpy(gLog[gLogIndex].Header.strLogType, pstrLogType);
	strcpy(gLog[gLogIndex].Header.strRegKey, pstrRegKey);
	
	printf("=================================================================\n");
	printf("gLog[%d].Header.strVersion = %s\n", gLogIndex, gLog[gLogIndex].Header.strVersion);
	printf("gLog[%d].Header.strDate = %s\n", gLogIndex, gLog[gLogIndex].Header.strDate);
	printf("gLog[%d].Header.strSystem = %s\n", gLogIndex, gLog[gLogIndex].Header.strSystem);
	printf("gLog[%d].Header.strSubSystem = %s\n", gLogIndex, gLog[gLogIndex].Header.strSubSystem);
	printf("gLog[%d].Header.strLogType = %s\n", gLogIndex, gLog[gLogIndex].Header.strLogType);
	printf("gLog[%d].Header.strRegKey = %s\n", gLogIndex, gLog[gLogIndex].Header.strRegKey);
	printf("=================================================================\n");
	return 0;
}

int setBodyData(char *pstrActionID, char *pstrActionType, char *pstrActionApp, char *pstrHostname, char *pstrJobID, int pJobStatus, int pJobElapsed)
{
	if(pstrActionID == NULL || pstrActionType == NULL || pstrActionApp == NULL || pstrHostname == NULL || pstrJobID == NULL) {
		fprintf(stderr, "Error : string parameters is null\t\t[File : %s, Line : %d, Function : %s]\n", __FILE__, __LINE__, __FUNCTION__);
		return -1;
	}
	
	if(gLog == NULL) {
		fprintf(stderr, "Error : gLog is null!\t\t[File : %s, Line : %d, Function : %s]\n", __FILE__, __LINE__, __FUNCTION__);
		return -1;
	}
	
	strcpy(gLog[gLogIndex].Body.strActionID, pstrActionID);
	strcpy(gLog[gLogIndex].Body.strActionType, pstrActionType);
	strcpy(gLog[gLogIndex].Body.strActionApp, pstrActionApp);
	strcpy(gLog[gLogIndex].Body.strHostname, pstrHostname);
	strcpy(gLog[gLogIndex].Body.strJobID, pstrJobID);
	gLog[gLogIndex].Body.nJobStatus = pJobStatus;
	gLog[gLogIndex].Body.nJobElapsed = pJobElapsed;

	printf("=================================================================\n");
	printf("gLog[%d].Body.strActionID = %s\n", gLogIndex, gLog[gLogIndex].Body.strActionID);
	printf("gLog[%d].Body.strActionType = %s\n", gLogIndex, gLog[gLogIndex].Body.strActionType);
	printf("gLog[%d].Body.strActionApp = %s\n", gLogIndex, gLog[gLogIndex].Body.strActionApp);
	printf("gLog[%d].Body.strHostname = %s\n", gLogIndex, gLog[gLogIndex].Body.strHostname);
	printf("gLog[%d].Body.strJobID = %s\n", gLogIndex, gLog[gLogIndex].Body.strJobID);
	printf("gLog[%d].Body.nJobStatus = %d\n", gLogIndex, gLog[gLogIndex].Body.nJobStatus);
	printf("gLog[%d].Body.nJobElapsed = %d\n", gLogIndex, gLog[gLogIndex].Body.nJobElapsed);
	printf("=================================================================\n");


	return 0;
}

/*
json_t* generateHeaderJSONObject(json_t *pJsonObj, char *pVersion, char *pSystem, char *pTime, char *pStatus)
{
	json_object_set(json_object_get(pJsonObj, "Header"), "Version", json_string(pVersion));
	json_object_set(json_object_get(pJsonObj, "Header"), "Date", json_string(pTime));
	json_object_set(json_object_get(pJsonObj, "Header"), "System", json_string(pSystem));
	json_object_set(json_object_get(pJsonObj, "Header"), "Job-State", json_string(pStatus));
	
	return pJsonObj;
}
*/
json_t* generateHeaderJSONObject(json_t *pJsonObj, HEADER header)
{
	json_object_set(json_object_get(pJsonObj, "Header"), "version", json_string(header.strVersion));
	json_object_set(json_object_get(pJsonObj, "Header"), "date", json_string(header.strDate));
	json_object_set(json_object_get(pJsonObj, "Header"), "system", json_string(header.strSystem));
	json_object_set(json_object_get(pJsonObj, "Header"), "subsystem", json_string(header.strSubSystem));
	json_object_set(json_object_get(pJsonObj, "Header"), "logtype", json_string(header.strLogType));
	json_object_set(json_object_get(pJsonObj, "Header"), "regkey", json_string(header.strRegKey));
	
	return pJsonObj;
}

json_t* generateBodyJSONArray(json_t *pJsonObj, json_t *pJsonArray, char *pApplication, char *pJobID, char *pStatus)
{
	json_t *jsonObj = json_object();
	
	json_object_set_new(jsonObj, "Application", json_string(pApplication));
  json_object_set_new(jsonObj, "Job-ID", json_string(pJobID));
  json_object_set_new(jsonObj, "Status", json_string(pStatus));
	json_array_append_new(pJsonArray, jsonObj);
	json_object_set(pJsonObj, "Body", pJsonArray);
  
	return pJsonObj;
}

/*
json_t* generateBodyJSONObject(json_t *pJsonObj, char *pApplication, char *pJobID)
{	
	json_object_set(json_object_get(pJsonObj, "Body"), "application", json_string(pApplication));
	json_object_set(json_object_get(pJsonObj, "Body"), "job-ID", json_string(pJobID));
	return pJsonObj;
} 
*/

json_t* generateBodyJSONObject(json_t *pJsonObj, BODY body)
{	
	json_object_set(json_object_get(pJsonObj, "Body"), "actionid", json_string(body.strActionID));
	json_object_set(json_object_get(pJsonObj, "Body"), "actiontype", json_string(body.strActionType));
	json_object_set(json_object_get(pJsonObj, "Body"), "actionapp", json_string(body.strActionApp));
	json_object_set(json_object_get(pJsonObj, "Body"), "hostname", json_string(body.strHostname));
	json_object_set(json_object_get(pJsonObj, "Body"), "jobid", json_string(body.strJobID));
	json_object_set(json_object_get(pJsonObj, "Body"), "jobstatus", json_integer(body.nJobStatus));
	json_object_set(json_object_get(pJsonObj, "Body"), "jobelapsed", json_integer(body.nJobElapsed));
	
	return pJsonObj;
} 


char* generate_json_string()
{
	int i = 0;
	char *pString = NULL;
	//json_t *jsonObj;
	//json_t *jsonArray;

	
	/*
	jsonObj = json_object();
	jsonArray = json_array(); 
	
	json_object_set_new(jsonObj, "Header", json_object());
	json_object_set_new(jsonObj, "Body", jsonArray);

	
	jsonObj = generateHeaderJSONObject(jsonObj, "1.0", "GTS", gLog->Header.strTime, gLog->Header.strStatus);
	for(i = 0; i < gLog->nBody; i++) 
	{
		jsonObj = generateBodyJSONArray(jsonObj, jsonArray, gLog->Body[i].strApplication, 
										gLog->Body[i].strJobID, gLog->Body[i].strStatus);
	}
	*/
	
	/*
	jsonObj = generateHeaderJSONObject(jsonObj, "1.0", "GTS", "2013-10-21 03:01:59", "01");

	jsonObj = generateBodyJSONArray(jsonObj, jsonArray, "buoy_encode", 
									"20131021031321", "01");
	jsonObj = generateBodyJSONArray(jsonObj, jsonArray, "buoy_encode", 
									"20131021051231", "03");
	*/

	// JSON 루트
	//json_t *jsonRootObj = json_object();
	json_t *jsonArray = json_array();

	json_t *jsonObj1 = json_object();
	json_object_set_new(jsonObj1, "Header", json_object());
	json_object_set_new(jsonObj1, "Body", json_object());

	generateHeaderJSONObject(jsonObj1, gLog[0].Header);
	generateBodyJSONObject(jsonObj1, gLog[0].Body);

	json_array_append_new(jsonArray, jsonObj1);

	json_t *jsonObj2 = json_object();
	json_object_set_new(jsonObj1, "Header", json_object());
	json_object_set_new(jsonObj1, "Body", json_object());

	generateHeaderJSONObject(jsonObj1, gLog[1].Header);
	generateBodyJSONObject(jsonObj1, gLog[1].Body);

	json_array_append_new(jsonArray, jsonObj1);

	//json_object_set_new(jsonRootObj, "", jsonArray);


	pString = json_dumps(jsonArray, 0);
	
	//json_decref(jsonArray);
	json_decref(jsonArray);
	
	return pString;
}


int main(int argc, char *argv[])
{

	initalize();
	gLogIndex = 0;

	setHeaderData(LOGVERSION, "2013-10-31 12:10:10", LOGSYSTEM, LOGSUBSYSTEM, LOGTYPE, LOGREGKEY);
	setBodyData(LOGACTIONID, LOGACTIONTYPE, "buoy_encode", LOGHOSTNAME, LOGJOBID, 1, LOGJOBELAPSED);

	gLogIndex = 1;
	setHeaderData(LOGVERSION, "2013-10-31 12:10:11", LOGSYSTEM, LOGSUBSYSTEM, LOGTYPE, LOGREGKEY);
	setBodyData(LOGACTIONID, LOGACTIONTYPE, "buoy_encode", LOGHOSTNAME, LOGJOBID, 3, LOGJOBELAPSED);
	
	//TestJsonObject();
	//char *retString = TestJsonArray();
	
	char *retString = generate_json_string();
	printf("%s\n", retString);

	finalize();

	return 0;
}



//void TestJsonObject()
//{
//	char *pString = NULL;
//	
//	json_t *jsonObj = json_object();
//	json_object_set_new(jsonObj, "Header", json_object());
//	json_object_set_new(jsonObj, "Body", json_object());
//	
//	jsonObj = generateHeaderJSONObject(jsonObj, "1.0", "GTS", "2013-10-21 03:01:59", "01");
//	jsonObj = generateBodyJSONObject(jsonObj, "buoy_encode", "20131021031321");
//
//	
//	pString = json_dumps(jsonObj, 0);
//	
//	json_decref(jsonObj);
//	
//	printf("%s\n", pString);
//}

//char* TestJsonArray()
//{
//	char *pString = NULL;
//	json_t *jsonObj;
//	json_t *jsonArray;
//	
//	jsonObj = json_object();
//	jsonArray = json_array(); 
//	
//	json_object_set_new(jsonObj, "Header", json_object());
//	json_object_set_new(jsonObj, "Body", jsonArray);
//	
//	jsonObj = generateHeaderJSONObject(jsonObj, "1.0", "GTS", "2013-10-21 03:01:59", "01");
//	jsonObj = generateBodyJSONArray(jsonObj, jsonArray, "buoy_encode", "20131021031321", "01");
//	jsonObj = generateBodyJSONArray(jsonObj, jsonArray, "buoy_encode", "20131021051231", "03");
//	
//	pString = json_dumps(jsonObj, 0);
//	
//	json_decref(jsonArray);
//	json_decref(jsonObj);
//	
//	return pString;
//}
