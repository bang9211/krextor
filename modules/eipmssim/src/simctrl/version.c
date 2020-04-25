#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


static char _g_version_str[] = {
	/* �ֽ� ������ �ۼ����ּ���. */
	"Release version 1.3.2\n"
};
static char _g_history_str[] = {
	/* ���� ������Ʈ �̷��� �ۼ����ּ���. ���� �̷� �Ʒ� �߰����ּ���. */
	"2014.04.17 Release version 1.0.0 - Common Simulator Controller ���� ���� ����\n"
    "                                 - Scenario load / unload / display / run\n"
	"                                 - Simulation result report.\n"
	"2014.04.23 Release version 1.0.1 - command ��� �߰� (remote host, sleep)\n"
	"                                 - report result ��� ���� (display scenario index)\n"
	"2014.05.09 Release version 1.0.2 - Control signals �߰� (signal �����Ҷ����� ������ ��� ������ �� ����)\n"
	"2014.05.29 Release version 1.0.3 - report plugin ��� �߰� (��ϵ� �÷����ο��� report comapre function ȣ��)\n"
	"2014.06.03 Release version 1.1.0 - RMI ���� ��� �߰� (console Ȥ�� scenario ���� rmi ���� ����)\n"
	"2014.06.12 Release version 1.1.1 - report result ��� ���� (�񱳰�� ���̳��� �κ��� ����Ʈ�Ҷ� ���ι�ȣ�� ���)\n"
	"2014.06.12 Release version 1.1.2 - configuration file-report �߰� (�ó����� ���� ����� ���Ͽ� �߰� ������� ���� ����)\n"
	"2015.06.08 Release version 1.1.3 - configuration define �߰� (rmi,command���� ����� keyword�� ���� replace ó��)\n"
	"2015.10.06 Release version 1.1.4 - configuration scenario-timeout, command timeout �߰� (�ó����� ���� Ÿ�Ӿƿ� ����)\n"
	"2015.11.04 Release version 1.1.5 - configuration replace-keyword �߰� (��� �м� �� ����� ������ �������� ������ keyword ���� ����)\n"
	"2016.01.11 Release version 1.1.6 - max size ���� (group : 20 -> 30, scenario : 100, command : 8 -> 16, rmi : 10)\n"
	"2016.01.21 Release version 1.1.7 - sleep field max value ���� (60 -> 360 sec)\n"
	"2016.07.22 Release version 1.1.8 - configuration realtime-display �߰� (�ó����� ���� ���� ���� ��� ���)\n"
	"2016.08.05 Release version 1.2.0 - scenario ���� ��� �߰� (prompt �󿡼� �����ϴ� ���� ��� �ܿ� simctrl ����� ���ÿ� ����Ǵ� ��� �߰�)\n"
	"2016.08.11 Release version 1.2.1 - command ��� �߰� (remote passwd) > ������ ��� remote cmd ó�� �� expect�� �̿��Ͽ� password ó����\n" 
	"2016.09.26 Release version 1.2.2 - scenario load ��� �߰� (simctrl ����� ���ÿ� scenario load ����)\n"
	"2016.11.15 Release version 1.2.3 - report result ��� ���� (sipp/diameter �޽��� �м� ��� ���� - �޽��� ���� �м�, ���/AVP ��� �м�)\n"
	"2018.03.27 Release version 1.2.4 - report result ��� ���� (diff ���� ����)\n"
	"2018.07.11 Release version 1.2.5 - report result ��� ���� (ignore keyword �� ���� ����)\n"
	"2018.11.02 Release version 1.2.6 - rmi ��� ���� �����Ͱ� Ŭ ��� ���� ó�� ���� ���� ó�� ���� ����\n"
	"2018.11.23 Release version 1.2.7 - report result ��� ���� (diff ���� ���� - sip \\r\\n ȭ�� ��½� \\r ����)\n"
	"2019.04.03 Release version 1.2.8 - command ��� ���� (script field�� path / command�� ������ ��� ����)\n"
	"                                 - config ��� ���� (define, replace-keyword �� SYSTEM(\"cmd\") ��� �� system call ���� ����� ��ȯ����)\n"
	"2019.04.23 Release version 1.2.9 - max size ���� (command : 16 -> 30, rmi : 10 -> 20)\n"
	"2019.05.28 Release version 1.3.0 - command ��� �߰� (scen-reload command �߰� - ������ �ε��� �ó����� unload �� ���� ���ο� �ó����� �ε�)\n"
	"2019.07.23 Release version 1.3.1 - sipp �ó����� ���� �� ��� ��� ���� �߸� ���� ����\n"
	"2020.02.24 Release version 1.3.2 - report result ��� ���� (replace-keyword ���� ignore keyword ����� ��� replace �������� �ʴ� ���� ����)\n"
	//" TODO - �׽ñ⵿�ؾ��� ���� ���μ���(�밳 �ܺο��� ��� �ù�������) ���� ���
};


void __version_check__( int argc, char *argv[] ) 
{
	int i, vflag = 0, hflag = 0;

	if( argc >= 2 ) {
		for( i = 1 ; i < argc ; i ++ ) {
			if( !strcasecmp(argv[i], "--version") ) vflag = 1;
			else if( !strcasecmp(argv[i], "--history") ) hflag = 1;
		}
		if( vflag ) printf(_g_version_str);
		if( hflag ) printf(_g_history_str);
		if( vflag || hflag ) exit(0);
	}
}
