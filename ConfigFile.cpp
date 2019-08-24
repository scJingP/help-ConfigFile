#include "ConfigFile.h"


//----------------------------------------------------------------------------
// ����ԭ�ͣ� CConfigFile::CConfigFile()
// �������ܣ� CConfigFile ���캯��
// ��������� ��
// ���������� ��
// �������أ� ��
// ע����� ��
//----------------------------------------------------------------------------
CConfigFile::CConfigFile()
{
	m_IsOpen = -1;
}

//----------------------------------------------------------------------------
// ����ԭ�ͣ� CConfigFile::CConfigFile(char *pConfigFileName)
// �������ܣ� CConfigFile ���캯��
// ��������� char *pConfigFileName  �����ļ���
// ���������� ��
// �������أ� ��
// ע����� ��
//----------------------------------------------------------------------------
CConfigFile::CConfigFile(char *pConfigFileName)
{
	m_IsOpen = -1;
	Open(pConfigFileName);
}

//----------------------------------------------------------------------------
// ����ԭ�ͣ� CConfigFile::~CConfigFile()
// �������ܣ� CConfigFile ��������
// ��������� ��
// ���������� ��
// �������أ� ��
// ע����� ��
//----------------------------------------------------------------------------
CConfigFile::~CConfigFile()
{
	if (m_IsOpen == 0)
		m_CfgFile.close();
}

//----------------------------------------------------------------------------
// ����ԭ�ͣ� short  CConfigFile::Open(char *ConfigFileName)
// �������ܣ� ��ָ���������ļ�
// ��������� char *ConfigFileName  �����ļ���
// ���������� ��
// �������أ� 0  ���ļ��ɹ�
//            -1 ���ļ�ʧ��
// ע����� ��
//----------------------------------------------------------------------------
short  CConfigFile::Open(char *ConfigFileName)
{
	//����Ѿ���һ���ļ��򿪣����ȹر�
	if ( m_IsOpen == 0 )
	{
		m_CfgFile.close();
		m_IsOpen = -1;
	}
	//��ָ���������ļ���ʧ�ܷ���-1
	m_CfgFile.open(ConfigFileName,ios::in);
	if( !m_CfgFile )
		return -1;

	m_IsOpen = 0;
	return 0;
}

//----------------------------------------------------------------------------
// ����ԭ�ͣ� void CConfigFile::Close()
// �������ܣ� �ر������ļ�
// ��������� ��
// ���������� ��
// �������أ� ��
// ע����� ��
//----------------------------------------------------------------------------
void CConfigFile::Close()
{
	if ( m_IsOpen == 0 )
		m_CfgFile.close();
}

//----------------------------------------------------------------------------
// ����ԭ�ͣ� short  CConfigFile::GetValue(char *Session,char *ValueName,char *RetStr)
// �������ܣ� �������ļ��ж���ָ���ı�����ֵ
// ��������� char *Session   �������ڽڵ�����
//            char *ValueName ����������
//            char *RetStr    ��ŷ����ַ����Ŀռ�ָ��
// ���������� ��
// �������أ� 0  ����ָ��������ֵ
//            -1 δ����ָ��������ֵ
// ע����� ��
//----------------------------------------------------------------------------
short  CConfigFile::GetValueForSession(char *Session, list<string> &lst){
	char          tmpstr[MAX_LINE_LENGTH];
	short          Flag;

	m_CfgFile.seekg(0,ios::beg);
	//�ҵ���Ӧ�Ľ�
	Flag = -1;
	while(!m_CfgFile.eof())
	{
		memset(tmpstr,0,MAX_LINE_LENGTH);
		m_CfgFile.getline(tmpstr,MAX_LINE_LENGTH);
		if ( GetSession(tmpstr,Session) == 0 )
		{
			Flag = 0;
			break;
		}
	}

	if ( Flag == -1 )
		return -1;

	//��ö�Ӧ�ı�����ֵ
	Flag = -1;
	while(!m_CfgFile.eof())
	{
		memset(tmpstr,0,MAX_LINE_LENGTH);
		m_CfgFile.getline(tmpstr,MAX_LINE_LENGTH);
		if(strlen(tmpstr) <= 0){
			break;
		}
		lst.push_back(tmpstr);
		Flag = 0;
	}

	return Flag;
}

//----------------------------------------------------------------------------
// ����ԭ�ͣ� short  CConfigFile::GetValue(char *Session,char *ValueName,char *RetStr)
// �������ܣ� �������ļ��ж���ָ���ı�����ֵ
// ��������� char *Session   �������ڽڵ�����
//            char *ValueName ����������
//            char *RetStr    ��ŷ����ַ����Ŀռ�ָ��
// ���������� ��
// �������أ� 0  ����ָ��������ֵ
//            -1 δ����ָ��������ֵ
// ע����� ��
//----------------------------------------------------------------------------
short  CConfigFile::GetValue(char *Session,char *ValueName,char *RetStr)
{
	char          tmpstr[MAX_LINE_LENGTH];
	short          Flag;
	short         Ret;

	m_CfgFile.seekg(0,ios::beg);
	//�ҵ���Ӧ�Ľ�
	Flag = -1;
	while(!m_CfgFile.eof())
	{
		m_CfgFile.getline(tmpstr,MAX_LINE_LENGTH);
		if ( GetSession(tmpstr,Session) == 0 )
		{
			Flag = 0;
			break;
		}
	}

	if ( Flag == -1 )
		return -1;

	//��ö�Ӧ�ı�����ֵ
	Flag = -1;
	while(!m_CfgFile.eof())
	{
		m_CfgFile.getline(tmpstr,MAX_LINE_LENGTH);
		if(strlen(tmpstr) <= 0){
			break;
		}
		Ret = GetContent(tmpstr,ValueName,RetStr);
		if (  (Ret == IS_COMMENT) || (Ret == NOT_FOUND))   
			continue;
		else if( Ret == IS_SESSION )
			break;
		Flag = 0;
		//    cerr << Session << " " << ValueName << "=" << RetStr << endl;
		break;
	}

	return Flag;
}

//----------------------------------------------------------------------------
// ����ԭ�ͣ� short CConfigFile::GetSession(char *pStr,char *SessionName)
// �������ܣ� ��ָ�����ַ����в���һ����(��[]�е��ַ���)������
// ��������� char *pStr ָ�����ַ���
//            char *SessionName ��Ҫ���ҵĽ���
// ���������� ��
// �������أ� 0  �ҵ�ָ���Ľ���
//            -1 δ�ҵ�ָ���Ľ���
// ע����� ��
//----------------------------------------------------------------------------
short CConfigFile::GetSession(char *pStr,char *SessionName)
{
	char TmpStr[100];
	int i=0;
	int j=0;

	if( pStr[i] == '[' ){
		i++; //����'['
	}else{
		return -1;  //���ǽ���
	}
	
	//��ý���
	while( pStr[i] != ']' )
	{
		TmpStr[j] = pStr[i];
		i++;
		j++;
	}
	TmpStr[j] = 0;

	if( strcmp(SessionName, TmpStr) != 0 )
		return -1; //����ָ���Ľ���

	return 0;
}

//----------------------------------------------------------------------------
// ����ԭ�ͣ� short CConfigFile::GetContent(char *pStr,char *ValueName,char *RetStr)
// �������ܣ� ��ָ�����ַ����л�ñ�����ֵ
// ��������� char *pStr      ָ�����ַ���
//            char *ValueName ����������
//            char *RetStr    ��ŷ����ַ����Ŀռ�ָ��
// ���������� ��
// �������أ� IS_VALUE ����������ֵ
//            IS_COMMENT ��ע��
//            IS_SESSION �ǽ���
//            NOT_FOUND  û���ҵ�ָ���ı�����
// ע����� ��
//----------------------------------------------------------------------------
short CConfigFile::GetContent(char *pStr,char *ValueName,char *RetStr)
{
	char TmpStr[100];
	int i=0;
	int j = 0;
	RetStr[0] = 0;
	
	if( pStr[i] == '#' ) return IS_COMMENT;  //��ע��
	if( pStr[i] == '[' ) return IS_SESSION;  //�ǽ���
	if( pStr[i] == 0   ) return NOT_FOUND;   //�ѵ���β��δ�ҵ�

	//��ñ�����
	//  while( pStr != ' ' &amp;&amp; pStr != '=' &amp;&amp; pStr != 0)
	//modified by luxd
	while( pStr[i] !='\t' && pStr[i] != '=' && pStr != 0)
	{
		TmpStr[j] = pStr[i];
		i++;
		j++;
	}
	TmpStr[j] = 0;

	if( strcmp(TmpStr,ValueName) != 0 )
		return NOT_FOUND;  //����ָ���ı���

	while( pStr[i]=='\t' || pStr[i] == '=' ) i++; //����'=' �� tab

	//��ñ�����ֵ
	j=0;
	while( pStr[i] != '#' && pStr[i] != '\t' && pStr[i] != '\r' && pStr[i] != '\n') //10 ���� \n 	13 �س� \r		9 Tab \t
	{
		RetStr[j] = pStr[i];
		i++;
		j++;
	}
	RetStr[j] = 0;

	return IS_VALUE;
}