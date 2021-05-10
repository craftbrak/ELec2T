import serial
import threading

from kivy.app import App
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.uix.image import Image
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.textinput import TextInput
from kivy.core.window import Window

FLAG_RESET = 1
FLAG_RESEND = 2
FLAG_SET_MAX = 4


class MyApp(App):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.port = serial.Serial('COM3')
        self.running = True
        self.people = 0
        self.max_people = 0

        #Précharge les images
        Image(source='vert.png')
        Image(source='rouge.png')

        self.thread = threading.Thread(target=self.read)
        self.thread.start()

    def on_request_close(self, *args):
        self.running = False
        self.port.close()
        self.thread.join()
        return False

    def read(self):
        self.write(FLAG_RESEND)
        while self.running:
            buf = self.port.read(3)
            if buf:
                flags, people, max_people = buf
                self.image.source = 'rouge.png' if flags else 'vert.png'
                self.people = people
                self.max_people = max_people
                self.label_actuel.text = f"Il y a {self.people} personnes dans le magasin !"
                self.label_limite.text = f"La limite du magasin est de {self.max_people} personnes !"
                print(f'recv {flags=} {people=} {max_people=}')

    def write(self, flags, max_people=0):
        self.port.write(bytes([flags, max_people]))

    def set_max_people(self, max_people):
        self.write(FLAG_SET_MAX | FLAG_RESEND, max_people)

    def send_reset(self):
        self.write(FLAG_RESET)

    def build(self):
        self.afficheur = BoxLayout(orientation='vertical')

        self.afficheur.add_widget(Label(text="A combien de personnes voulez-vous limiter l'accès ?"))

        self.nombre_client = TextInput(text="", input_type='number')
        self.afficheur.add_widget(self.nombre_client)

        self.boutton = Button(text="Définir nombre client")
        self.boutton.bind(on_press=self.voir)
        self.afficheur.add_widget(self.boutton)

        self.label_limite = Label(text=f"La limite du magasin est de {self.max_people} personnes")
        self.afficheur.add_widget(self.label_limite)

        self.label_actuel = Label(text=f"Il y a {self.people} personnes dans le magasin !")
        self.afficheur.add_widget(self.label_actuel)

        layout = BoxLayout(orientation='horizontal')
        button = Button(text='Reset')
        button.bind(on_press=lambda *a: self.send_reset())
        self.image = Image(source='vert.png')
        layout.add_widget(self.image)
        layout.add_widget(button)
        self.afficheur.add_widget(layout)

        Window.bind(on_request_close=self.on_request_close)

        return self.afficheur

    def voir(self, text):
        nombre = self.nombre_client.text.strip()
        if nombre == "":
            print("False")
        elif not nombre.isnumeric():
            print("Ce n'est pas un nombre !")
            self.label_limite.text = "Ce n'est pas un nombre !"
        else:
            self.set_max_people(int(self.nombre_client.text))


if __name__ == '__main__':
    MyApp().run()
