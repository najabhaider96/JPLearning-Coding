/*
Welcome to JP Learning
*/
char Str1[15];
char Str2[8] = {'a', 'r', 'd', 'u', 'i', 'n', 'o'};
char Str3[8] = {'a', 'r', 'd', 'u', 'i', 'n', 'o', '\0'};
char Str4[] = "arduino";
char Str5[8] = "arduino";
char Str6[15] = "arduino";
String Str7 = "arduino";
void setup() {
  Serial.begin(115200); 
}

void loop() {
  Serial.println(Str1);
  Serial.println(Str2);
  Serial.println(Str3);
  Serial.println(Str4);
  Serial.println(Str5);
  Serial.println(Str6);
  Serial.println(Str7);
  delay(1000);
}
