#define PORT_RAND (A0)
#define SERIAL_RATE (9600)
#define MAX_TYPE_CHAR (26 + 26 + 10 + 5) // 소문자 + 대문자 + 숫자 + 특수문자 
#define SERIAL_DEALY (50)

int nTotalScore = 0; //합산 스코어
int nTotalInputTime =  0; //총 입력시간 (ms)

///////////////////////////
// 초기화 부분(타자 초기화)///
///////////////////////////

void initRand() {
  int nVolt = analogRead(PORT_RAND);
  randomSeed(nVolt); // 난수를 초기화
}

void initGame() { //타자 초기화
  Serial.begin(SERIAL_RATE);
  initRand();
  nTotalScore = 0;
}

String getSerialInput() {
  String str; //length() ==0
  while (Serial.available() > 0)
  {
    char c = Serial.read();
    str += c;
    delay(SERIAL_DEALY);
  }
  return str;
}

//scnaf: Serial -> String -> int
String scanStr() {
  while (Serial.available() <= 0);
  return getSerialInput();
}

int scanInt() {
  String str = scanStr();
  return str.toInt();
}

void clearSerialInput() {
  while (Serial.available() > 0)
  {
    Serial.read();
    delay(SERIAL_DEALY);
  }
}
///////////////////////////
//랜덤 문장 만들기 부분 (난수 문장)
///////////////////////////

char english(int nRand) { //소문자
  if (nRand <= 25) return (char)(nRand + 'a');
  else return ' ';
}

char bigenglish(int nRand) { //대문자 + 소문자
  if (nRand <= 25) return (char)(nRand + 'a');
  else if (25 < nRand && nRand <= 51) return (char)(nRand - 26 + 'A');
  else return ' ';
}

char numenglish(int nRand) {//대문자 + 소문자 + 숫자
  if (nRand <= 25) return (char)(nRand + 'a');
  else if (25 < nRand && nRand <= 51) return (char)(nRand - 26 + 'A');
  else if (51 < nRand && nRand <= 61) return (char)(nRand - 52 + '0');
  else return ' ';
}

char specialnumenglish(int nRand) {//대문자 + 소문자 + 숫자 + 특수문자 + 공백
  if (nRand <= 25) return (char)(nRand + 'a');
  else if (25 < nRand && nRand <= 51) return (char)(nRand - 26 + 'A');
  else if (51 < nRand && nRand <= 61) return (char)(nRand - 52 + '0');
  else if (nRand == 62) return '.';
  else if (nRand == 63) return ',';
  else if (nRand == 64) return '?';
  else if (nRand == 65) return '!';
  else if (nRand == 66) return ' ';
  else return ' ';
}

String Randone(int nRandSize) {
  String sRand;
  for (int i = 0; i < nRandSize; i++) { //반복
    int nRand = random(26); //0~127까지 랜덤으로
    char ch = english(nRand); // int를 char로 바꿈
    sRand += ch;
  }
  return sRand;
}

String Randtwo(int nRandSize) {
  String sRand;
  for (int i = 0; i < nRandSize; i++) { //반복
    int nRand = random(0, 52); //0~127까지 랜덤으로
    char ch = bigenglish(nRand); // int를 char로 바꿈
    sRand += ch;
  }
  return sRand;
}

String Randthree(int nRandSize) {
  String sRand;
  for (int i = 0; i < nRandSize; i++) { //반복
    int nRand = random(0, 62); //0~127까지 랜덤으로
    char ch = numenglish(nRand); // int를 char로 바꿈
    sRand += ch;
  }
  return sRand;
}

String Randfour(int nRandSize) {
  String sRand;
  for (int i = 0; i < nRandSize; i++) { //반복
    int nRand = random(MAX_TYPE_CHAR); //0~127까지 랜덤으로
    char ch = specialnumenglish(nRand); // int를 char로 바꿈
    sRand += ch;
  }
  return sRand;
}

//////////////////
// Typing Methods
//////////////////

String getUserInput(int& nInputTime) { //사용자입력 , 입력 시간 측정
  nInputTime = 0;
  clearSerialInput();
  int nStartTime = millis();
  String sInput = scanStr();
  int nStopTime = millis();
  nInputTime = nStopTime - nStartTime;
  return sInput;
}

//return score
int compareSentences(String sRand, String sInput) { //문장비교
  int nScore = 0;
  int nRandLen = sRand.length();
  int nInputLen = sInput.length();
  int nMinLen = (nRandLen < nInputLen) ? nRandLen : nInputLen;
  for (int i = 0; i < nMinLen; i++) {
    if (sRand[i] == sInput[i]) nScore++;
    else nScore--;
  }
  return nScore - abs(nRandLen - nInputLen);
}

// nTotalInputTime ,nTotalScore 를 이용하여 통계처리
void updateStat(int nUserScore, int nInputTime) { //타자 통계처리
  nTotalScore += nUserScore;
  nTotalInputTime += nInputTime;
}

// nTotalInputTime ,nTotalScore 를 이용하여 통계출력
void printStat() { // 통계 출력
  double typeSpeed = nTotalScore / (double)(nTotalInputTime * 1e-3) * 60.;
  Serial.println("누적 점수 = " + String(nTotalScore));
  Serial.println("평균 분당 타자수 = " + String(typeSpeed));
}

void stopgame() {
  if (nTotalScore < 0) {
    Serial.println("GAME OVER 메인 메뉴로 돌아갑니다.");
    Serial.println("");
    maintitle();
  }
}

void tazagameone() {
  char num = Serial.read();
  if (num == '1') {
    Serial.print("문제 :");
    String sRand = Randone(10);
    Serial.println(sRand);
    int nInputTime = 0;
    String sInput = getUserInput(nInputTime);
    Serial.println(sInput);
    int nUserScore = compareSentences(sRand, sInput);
    Serial.println("획득 점수 :" + String(nUserScore));
    updateStat(nUserScore, nInputTime);
    printStat();
    Serial.println("입력 시간 : " + String(nInputTime * 1e-3) + "sec");
    Serial.println("------------------------");
    Serial.println("계속 하시려면 1번 메인화면으로 돌아가려면 2번을 입력해주세요.");
    Serial.println("");
    stopgame();
  }
  else if (num == '2') {
    Serial.println(" ");
    maintitle();
    while (1) {
      mainmenu();
    }
  }
}

void tazagametwo() {
  char num = Serial.read();
  if (num == '1') {
    Serial.print("문제 :");
    String sRand = Randtwo(10);
    Serial.println(sRand);
    int nInputTime = 0;
    String sInput = getUserInput(nInputTime);
    Serial.println(sInput);
    int nUserScore = compareSentences(sRand, sInput);
    Serial.println("획득 점수 :" + String(nUserScore));
    updateStat(nUserScore, nInputTime);
    printStat();
    Serial.println("입력 시간 : " + String(nInputTime * 1e-3) + "sec");
    Serial.println("------------------------");
    Serial.println("계속 하시려면 1번 메인화면으로 돌아가려면 2번을 입력해주세요.");
    Serial.println("");
    stopgame();
  }
  else if (num == '2') {
    Serial.println(" ");
    maintitle();
    while (1) {
      mainmenu();
    }
  }
}
void tazagamethree() {
  char num = Serial.read();
  if (num == '1') {
    Serial.print("문제 :");
    String sRand = Randthree(10);
    Serial.println(sRand);
    int nInputTime = 0;
    String sInput = getUserInput(nInputTime);
    Serial.println(sInput);
    int nUserScore = compareSentences(sRand, sInput);
    Serial.println("획득 점수 :" + String(nUserScore));
    updateStat(nUserScore, nInputTime);
    printStat();
    Serial.println("입력 시간 : " + String(nInputTime * 1e-3) + "sec");
    Serial.println("------------------------");
    Serial.println("계속 하시려면 1번 메인화면으로 돌아가려면 2번을 입력해주세요.");
    Serial.println("");
    stopgame();
  }
  else if (num == '2') {
    Serial.println(" ");
    maintitle();
    while (1) {
      mainmenu();
    }
  }
}

void tazagamefour() {
  char num = Serial.read();
  if (num == '1') {
    Serial.print("문제 :");
    String sRand = Randfour(10);
    Serial.println(sRand);
    int nInputTime = 0;
    String sInput = getUserInput(nInputTime);
    Serial.println(sInput);
    int nUserScore = compareSentences(sRand, sInput);
    Serial.println("획득 점수 :" + String(nUserScore));
    updateStat(nUserScore, nInputTime);
    printStat();
    Serial.println("입력 시간 : " + String(nInputTime * 1e-3) + "sec");
    Serial.println("------------------------");
    Serial.println("계속 하시려면 1번 메인화면으로 돌아가려면 2번을 입력해주세요.");
    Serial.println("");
    stopgame();
  }
  else if (num == '2') {
    Serial.println(" ");
    maintitle();
    while (1) {
      mainmenu();
    }
  }
}

void practiceMode() { //입력 속도 체크
  int nInputTime = 0;
  String sInput = getUserInput(nInputTime);
  Serial.println(sInput);
  Serial.println("");
  Serial.println("입력 시간 : " + String(nInputTime * 1e-3) + "sec");
  Serial.println("");
  Serial.println("1 : 재측정");
  Serial.println("2 : 메인 화면으로 이동");
  Serial.println("");
}

void maintitle() {
  Serial.println("┌────────────────영어 타자 게임────────────────┐");
  Serial.println("");
  Serial.println(" 　      아래의 4개의 난이도중 하나를 선택해주세요");
  Serial.println("");
  Serial.println("　 　       1.영어 소문자 타자 연습게임");
  Serial.println("　　        2.영어 대문자 소문자만 나오는 타자게임");
  Serial.println("　　        3.영어 대문자 소문자 숫자만 나오는 타자게임");
  Serial.println("　　        4.영어 대문자 소문자 숫자 특수문자 공백까지 나오는 타자게임");
  Serial.println("　　        5.타자 입력 시간체크");
  Serial.println("　　        6.종료");
  Serial.println("");
  Serial.println("                                                         MADE BY TS");
  Serial.println("└────────────────────────────────────────┘");
}

void mainmenu() {
  char num = Serial.read();
  if (num == '1') {
    Serial.println("1.영어 소문자 타자 연습게임");
    Serial.println(" ");
    Serial.println("* 시작하시려면 1번 메인화면으로 돌아가려면 2번을 입력해주세요.\n* 점수가 마이너스로 떨어지면 GAME OVER 됩니다.");
    Serial.println(" ");
    while (1) {
      tazagameone();
      if (nTotalScore < 0) {
        nTotalScore = 0;
        break;
      }
    }
  }
  else if (num == '2') {
    Serial.println("2.대문자와 소문자가 함께 나오는 연습게임");
    Serial.println(" ");
    Serial.println("* 시작하시려면 1번 메인화면으로 돌아가려면 2번을 입력해주세요.\n* 점수가 마이너스로 떨어지면 GAME OVER 됩니다.");
    Serial.println(" ");
    while (1) {
      tazagametwo();
      if (nTotalScore < 0) {
        nTotalScore = 0;
        break;
      }
    }
  }
  else if (num == '3') {
    Serial.println("3.영어 대문자 소문자 숫자만 나오는 타자게임");
    Serial.println(" ");
    Serial.println("* 시작하시려면 1번 메인화면으로 돌아가려면 2번을 입력해주세요.\n* 점수가 마이너스로 떨어지면 GAME OVER 됩니다.");
    Serial.println(" ");
    while (1) {
      tazagamethree();
      if (nTotalScore < 0) {
        nTotalScore = 0;
        break;
      }
    }
  }
  else if (num == '4') {
    Serial.println("4.영어 대문자 소문자 숫자 특수문자 공백까지 나오는 타자게임");
    Serial.println(" ");
    Serial.println("* 시작하시려면 1번 메인화면으로 돌아가려면 2번을 입력해주세요.\n* 점수가 마이너스로 떨어지면 GAME OVER 됩니다.");
    Serial.println(" ");
    while (1) {
      tazagamefour();
      if (nTotalScore < 0) {
        nTotalScore = 0;
        break;
      }
    }
  }

  else if (num == '5') {
    Serial.println(" ");
    Serial.println("1 : 타자 입력 시간 측정");
    Serial.println("2 : 메인 화면으로 이동");
    Serial.println(" ");

    while (1) {
      char num = Serial.read();
      if (num == '1') {
        Serial.println("타자 입력 시간을 체크해보세요.");
        Serial.println(" ");
        practiceMode();
      }
      else if (num == '2')
      {
        Serial.println("메인 화면으로 이동합니다.");
        Serial.println(" ");
        maintitle();
        break;
      }
    }
  }
  else if (num == '6') {
    Serial.println("프로그램이 종료되었습니다.");
    Serial.end();
  }
}

void setup() {
  // put your setup code here, to run once:
  initGame();
  maintitle();
}

void loop() {
  // put your main code here, to run repeatedly:
  mainmenu();
}
