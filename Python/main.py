import serial, sys
import serial.tools.list_ports
import kivy
from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.textinput import TextInput
import threading


ser = serial.Serial('COM4', baudrate=9600, timeout=1)


def fct():
    while True:
        msg = ser.readline().decode('ascii')
        if len(msg) != 0:
            msg_split = msg.split("*")
            print("Nombre de personne à l'intérieur : " + msg_split[1] + " " + msg_split[2])


class Threading(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

    def fonction_data(self):
        while True:
            msg = ser.readline().decode('ascii')
            if len(msg) != 0:
                msg_split = msg.split("*")
                print("Nombre de personne à l'intérieur : " + msg_split[1] + " " + msg_split[2])

    def ecrire(self, value):
        ser.write(bytes(value))


class MyApp(App):
    def build(self):
        self.afficheur = BoxLayout(orientation='vertical')

        self.afficheur.add_widget(Label(text="A combien de personnes voulez-vous limiter l'accès ?"))

        self.nombre_client = TextInput(text="")
        self.afficheur.add_widget(self.nombre_client)

        self.boutton = Button(text="Définir nombre client")
        self.boutton.bind(on_press=self.voir)
        self.afficheur.add_widget(self.boutton)

        self.label_limite = Label(text="La limite du magasin est de 5 personnes")
        self.afficheur.add_widget(self.label_limite)

        self.label_actuel = Label(text="Il y a 0 personnes dans le magasin !")
        self.afficheur.add_widget(self.label_actuel)

        thread = Threading()
        thread.start()

        return self.afficheur

    def voir(self, text):
        if(self.nombre_client.text == ""):
            print("False")
        else:
            print(self.nombre_client.text)
            self.label_limite.text = f"La limite du magasin est de {self.nombre_client.text} personnes !"


if __name__ == '__main__':
    MyApp().run()
