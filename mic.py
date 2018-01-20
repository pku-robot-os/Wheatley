#!/usr/bin/env python3
'''
Some specific Python libraries are required:
1. sudo pip install SpeechRecognition
2. Use "python -m speech_recognition" to test it out
If the test fails, maybe the following commands will help you:
3. sudo pip install -U pip
4. sudo apt-get install -qq python python-dev python-pip build-essential swig libpulse-dev
5. sudo pip install pocketsphinx
6. sudo apt-get install portaudio19-dev python-all-dev python3-all-dev && sudo pip install pyaudio
'''
import speech_recognition as sr
#while True:
r = sr.Recognizer()
with sr.Microphone() as source:
	audio = r.listen(source)
try:
	mic = r.recognize_google(audio)
except sr.UnknownValueError:
	mic = "Nothing"
except sr.RequestError as e:
	mic = "Error" 
