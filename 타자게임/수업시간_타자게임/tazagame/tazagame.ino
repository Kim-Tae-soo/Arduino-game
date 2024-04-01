#define PORT_RAND (A0)
#define SERIAL_RATE (9600)
#define MAX_TYPE_CHAR (26 + 5)
#define SERIAL_DELAY (50)

int nTotalScore = 0; //합산 스코어
long nTotalInputTime = 0; //총 입력시간 (ms)

///////////////////////////
// 초기화 부분 (타자 초기화)
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
    delay(SERIAL_DELAY);
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
    delay(SERIAL_DELAY);
  }
}


///////////////////////////
//랜덤 문장 만들기 부분 (난수 문장)
///////////////////////////

char toRandChar(int nRand) {
  if (nRand <= 25) return (char)(nRand + 'a');
  else if (nRand == 26) return '.';
  else if (nRand == 27) return ',';
  else if (nRand == 28) return '!';
  else if (nRand == 29) return '?';
  else if (nRand == 30) return ' ';
  else return ' ';
}

String makeRandSentence(int nRandSize) {
  String sRand;
  for (int i = 0; i < nRandSize; i++) { //반복
    int nRand = random(MAX_TYPE_CHAR); //0~127까지 랜덤으로
    char ch = toRandChar(nRand); // int를 char로 바꿈
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
  for (int i = 0; i < nMinLen; i++){
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
  double typeSpeed = nTotalScore/(double)(nTotalInputTime*1e-3)*60.;
  Serial.println("전체 스코어 = " + String(nTotalScore));
  Serial.println("평균 분당 타자수 = " + String(typeSpeed));

  
}

void setup() {
  // put your setup code here, to run once:
  initGame();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("문제 :");
  String sRand = makeRandSentence(10);
  Serial.println(sRand);
  int nInputTime = 0;
  String sInput = getUserInput(nInputTime);
  Serial.println(sInput);
  Serial.println("입력 걸린 시간 : " + String(nInputTime*1e-3) + "sec");
  int nUserScore = compareSentences(sRand, sInput);
  Serial.println("현재 점수 :" + String(nUserScore));
  updateStat(nUserScore, nInputTime);
  printStat();
  Serial.println("-------");
}
