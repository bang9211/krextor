
1. �ʰ��� ������ ��� ����

 SIMCTRL_SRC_HOME ȯ�� ���� ���� (ex> setenv SIMCTRL_SRC_HOME "$HOME/SIMCTRL"

 cd config
 ./configure
 cd ..
 make

 ** configure �ɼ� **
 --enable-64bit 64bit binary�� �������Ѵ�.
 --enable-xopen xopen Ȯ�� OS �Լ����� ����� �� �ֵ��� ������ �ɼ��� �����Ѵ�.
 --disable-debug  debug �ɼ� ���� binary�� �������Ѵ�.
 --disable-optimize ����ȭ �ɼ� ���� binary�� �������Ѵ�.
 --disable-warnging �����Ͻ� warnging�� ������� �ʴ´�.

 ** �÷����� configure ���� ��� **
    1) HP-UX IA-64(force���) :
	     // �⺻���� cc �ɼ����� a.out�� �ȳ��ͼ� ��������� CFLAGS�� �����ؾ���. 
         $> ./configure --enable-64bit CFLAGS='-Ae +DD64' CXXFLAGS='-AA +DD64'

    2) SUN, LINUX �� Ư���� �������� �׳� configure �����ϸ��.
