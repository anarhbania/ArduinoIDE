/*  This is simple program: Record time (us) between change logic (LOW or HIGH).
    We don't need know IR protocol, this is simple and universal program.

    What we need?
    a) 1x ArduinoNano
    b) 1x IR receiver
    c) ArduinoIDE (tested 1.8.19)

    Schematic:

    ArduinoNano     IR
             (2)----(Y)
             VCC----(R)
             GND----(G)

*/

int IR = 2; // (2) INPUT IR

const int arraySize = 300; // Max times to log.  If array is full, it will report the pattern to that point.
long arrayTime[arraySize]; // Array of times of when the IR pin changes state.

int count = 0; // Number of times logged to array.
long noChangeTime = 80000; // No change in IR pin for this duration (microseconds) will mark the end of pattern.

boolean logMicros = true;  //Flag to indicate if able to log times to array
long lastMicros = 0; // Previous array time entry

void setup()
{
  pinMode(IR, INPUT); // (2) INPUT IR

  attachInterrupt(digitalPinToInterrupt(IR), interupt, CHANGE); // Attach an interupt to log when the IR pin changes state

  Serial.begin(9600); // Run Serial

}

void loop()
{
  if (count > 0) // if numbers of times is not empty
  {
    if (!logMicros || ((micros() - lastMicros) > noChangeTime)) // if logMicros is false (have times in array) OR noChangeTime (timeout)
    {
      logMicros = false; // make sure no more data added to array while we report it

      count--; // remove last number of count, becouse this is timeout
      for (int i = 0; i < count; i++) // for 0 to number of count
      {
        arrayTime[i] = arrayTime[i + 1] - arrayTime[i]; // subtract times to get interval between change INPUT
        Serial.print(String(i + 1) + ") " + String(arrayTime[i]) + " us     "); // send to Serial arrayTime
        if (i % 2 == 1) Serial.println(""); // if can't completely i / 2, and have residue = 1, then send to Serial command new line
      }

      Serial.println(); // send to Serial command new line
      Serial.println(); // send to Serial command new line
      Serial.print("int timeIR[" + (String)count + "] = {"); // send to Serial build array
      for (int i = 0; i < count; i++) // for 0 to number of count
      {
        Serial.print(String(arrayTime[i])); // send to Serial arrayTime
        if (i != (count - 1))
        {
          Serial.print(", "); // send to Serial ,
        }
      }
      Serial.println("};"); // send to Serial }; (end array)

      logMicros = true; count = 0; // Reset array
    }
  }

  delay(10); // time for check logMicros
}

void interupt() // Interupt to log each time the IR signal changes state
{
  if (logMicros) // if logMicros is true (array is empty)
  {
    long m = micros(); // get time now

    if (count > 0 && ((m - lastMicros) > noChangeTime)) // if array is not empty AND noChangeTime (timeout)
    {
      logMicros = false; // no more data added to array
    }
    else
    {
      arrayTime[count] = m; // save time to array
      count++; // set next count
      lastMicros = m; // save last time

      if (count >= arraySize) // if count is bigger then arraySize
      {
        logMicros = false; // no more data added to array
      }
    }
  }
}
