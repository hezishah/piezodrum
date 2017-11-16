int noteOn = 144;
int noteOff = 128;
int piezo[] = {A0, A1, A2, A3};
int pState[] = {0, 0, 0, 0};
int pMin[] = {0, 0, 0, 0};
int pMax[] = {0, 0, 0, 0};
int pCount[] = {0, 0, 0, 0};
int numPiezos = 4;
int minDuration=20;
int minOffDuration=250;
int threshold = 3;//anything over fifty means we've hit the piezo

void setup(){
  Serial.begin(19200);
}

void loop(){
  for(int pIndex=0;pIndex<numPiezos/*sizeof(piezo)/sizeof(int)*/;pIndex++)
  {
    int piezoVal = analogRead(piezo[pIndex]);
    if(pState[pIndex]==2)
    {
      if(pCount[pIndex]>0)
         pCount[pIndex]--;
      else
         pState[pIndex]=0;
    }
    else
    {
      if (piezoVal>threshold)
      {
        ++pCount[pIndex];
        if(pMax[pIndex]==0)
        {
          pMin[pIndex] = pMax[pIndex] = piezoVal;
        }
        else
        {
          if(piezoVal > pMax[pIndex])
           pMax[pIndex] = piezoVal;
          if(pCount[pIndex]==(minDuration/numPiezos))
          {
            byte velocity = map(pMax[pIndex], 0, 256, 50, 127);
            MIDImessage(noteOn, 36+(pIndex*4), velocity);
            pState[pIndex]=1;
          }
        }
      }
      else
      {
        if(pState[pIndex]==1)
        {
          MIDImessage(noteOff, 36+(pIndex*4), 0);
          pState[pIndex]=2;
          pMin[pIndex]=0;
          pMax[pIndex]=0;
          pCount[pIndex]=minOffDuration/numPiezos;
        }
      }
    }
  }
}

//send MIDI message
void MIDImessage(byte command, byte data1, byte data2) {
  unsigned char buf[4]={command|9,data1,data2,0};
  Serial.flush();
  Serial.write(buf,3);
}

