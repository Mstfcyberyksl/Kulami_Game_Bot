# UI python tkinter ile yapılsın
# search
# check max point
import tkinter
import ctypes
from PIL import Image, ImageTk
from tkinter import PhotoImage

#black_image =
#select_image =
#empty_image =
root = tkinter.Tk()
root.title("Kulami Game")
root.attributes("-fullscreen",True)

image = Image.open("redball.png")
image2 = image.resize((80,80))
red_image = ImageTk.PhotoImage(image2)

image = Image.open("black-ball-png-9.png")
image2 = image.resize((80,80))
black_image = ImageTk.PhotoImage(image2)

functionbest = ctypes.CDLL("./kulami_game.so")
functionbest.best_place.argtypes = (ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_int)
functionbest.best_place.restype = (ctypes.POINTER(ctypes.c_int))
functionbest.main()

def button_click(x,y):
    buttons[(x,y)].config(image=red_image)
    print("PYTHON1")
    print(x,y)
    result = functionbest.best_place(2,x,y,5)
    print("PYTHON")
    print(result[0],result[1])
    buttons[(result[0],result[1])].config(image=black_image)
buttons = {}

for i in range(8):
    root.rowconfigure(i, weight=1)
    root.columnconfigure(i, weight=1)

#frame1
frame1 = tkinter.Frame(root, bd=10, relief='ridge')
frame1.grid(row=0, column=0, rowspan=2, columnspan=2, sticky="nsew")

for i in range(2):
    frame1.rowconfigure(i, weight=1)
    frame1.columnconfigure(i, weight=1)

button1 = tkinter.Button(frame1, image=None, command=lambda: button_click(0, 0))
button1.grid(row=0, column=0, sticky="nsew", padx=40, pady=40)
buttons[(0, 0)] = button1

button2 = tkinter.Button(frame1, image=None, command=lambda: button_click(0, 1))
button2.grid(row=0, column=1, sticky="nsew", padx=40, pady=40)
buttons[(0, 1)] = button2

button3 = tkinter.Button(frame1, image=None, command=lambda: button_click(1, 0))
button3.grid(row=1, column=0, sticky="nsew", padx=40, pady=40)
buttons[(1, 0)] = button3

button4 = tkinter.Button(frame1, image=None, command=lambda: button_click(1, 1))
button4.grid(row=1, column=1, sticky="nsew", padx=40, pady=40)
buttons[(1, 1)] = button4

#frame2

frame2 = tkinter.Frame(root, bd=10, relief='ridge')
frame2.grid(row=0, column=2, rowspan=2, columnspan=3, sticky="nsew")

for i in range(2):
    frame2.rowconfigure(i, weight=1)
    frame2.columnconfigure(i, weight=1)
frame2.columnconfigure(2, weight=1)

button5 = tkinter.Button(frame2, image=None, command=lambda: button_click(0, 2))
button5.grid(row=0, column=0, sticky="nsew", padx=40, pady=40)
buttons[(0, 2)] = button5

button6 = tkinter.Button(frame2, image=None, command=lambda: button_click(0, 3))
button6.grid(row=0, column=1, sticky="nsew", padx=40, pady=40)
buttons[(0, 3)] = button6

button7 = tkinter.Button(frame2, image=None, command=lambda: button_click(0, 4))
button7.grid(row=0, column=2, sticky="nsew", padx=40, pady=40)
buttons[(0, 4)] = button7

button8 = tkinter.Button(frame2, image=None, command=lambda: button_click(1, 2))
button8.grid(row=1, column=0, sticky="nsew", padx=40, pady=40)
buttons[(1, 2)] = button8

button9 = tkinter.Button(frame2, image=None, command=lambda: button_click(1, 3))
button9.grid(row=1, column=1, sticky="nsew", padx=40, pady=40)
buttons[(1, 3)] = button9

button10 = tkinter.Button(frame2, image=None, command=lambda: button_click(1, 4))
button10.grid(row=1, column=2, sticky="nsew", padx=40, pady=40)
buttons[(1, 4)] = button10

#frame3

frame3 = tkinter.Frame(root, bd=10, relief='ridge')
frame3.grid(row=0, column=5, rowspan=3, columnspan=2, sticky="nsew")

for i in range(2):
    frame3.rowconfigure(i, weight=1)
    frame3.columnconfigure(i, weight=1)
frame3.rowconfigure(2, weight=1)

button11 = tkinter.Button(frame3, image=None, command=lambda: button_click(0, 5))
button11.grid(row=0, column=0, sticky="nsew", padx=40, pady=40)
buttons[(0, 5)] = button11

button12 = tkinter.Button(frame3, image=None, command=lambda: button_click(0, 6))
button12.grid(row=0, column=1, sticky="nsew", padx=40, pady=40)
buttons[(0, 6)] = button12

button13 = tkinter.Button(frame3, image=None, command=lambda: button_click(1, 5))
button13.grid(row=1, column=0, sticky="nsew", padx=40, pady=40)
buttons[(1, 5)] = button13

button14 = tkinter.Button(frame3, image=None, command=lambda: button_click(1, 6))
button14.grid(row=1, column=1, sticky="nsew", padx=40, pady=40)
buttons[(1, 6)] = button14

button15 = tkinter.Button(frame3, image=None, command=lambda: button_click(2, 5))
button15.grid(row=2, column=0, sticky="nsew", padx=40, pady=40)
buttons[(2, 5)] = button15

button16 = tkinter.Button(frame3, image=None, command=lambda: button_click(2, 6))
button16.grid(row=2, column=1, sticky="nsew", padx=40, pady=40)
buttons[(2, 6)] = button16

#frame4

frame4 = tkinter.Frame(root, bd=10, relief='ridge')
frame4.grid(row=0, column=7, rowspan=3, columnspan=1, sticky="nsew")
for i in range(3):
    frame4.rowconfigure(i, weight=1)
frame4.columnconfigure(0, weight=1)

button17 = tkinter.Button(frame4, image=None, command=lambda: button_click(0, 7))
button17.grid(row=0, column=0, sticky="nsew", padx=40, pady=40)
buttons[(0, 7)] = button17

button18 = tkinter.Button(frame4, image=None, command=lambda: button_click(1, 7))
button18.grid(row=1, column=0, sticky="nsew", padx=40, pady=40)
buttons[(1, 7)] = button18

button19 = tkinter.Button(frame4, image=None, command=lambda: button_click(2, 7))
button19.grid(row=2, column=0, sticky="nsew", padx=40, pady=40)
buttons[(2, 7)] = button19

#frame5

frame5 = tkinter.Frame(root, bd=10, relief='ridge')
frame5.grid(row=2, column=0, rowspan=2, columnspan=1, sticky="nsew")

for i in range(2):
    frame5.rowconfigure(i, weight=1)
frame5.columnconfigure(0, weight=1)

button20 = tkinter.Button(frame5, image=None, command=lambda: button_click(2, 0))
button20.grid(row=0, column=0, sticky="nsew", padx=40, pady=40)
buttons[(2, 0)] = button20

button21 = tkinter.Button(frame5, image=None, command=lambda: button_click(2, 1))
button21.grid(row=1, column=0, sticky="nsew", padx=40, pady=40)
buttons[(2, 1)] = button21

#frame6

frame6 = tkinter.Frame(root, bd=10, relief='ridge')
frame6.grid(row=2, column=1, rowspan=2, columnspan=2, sticky="nsew")

for i in range(2):
    frame6.rowconfigure(i, weight=1)
    frame6.columnconfigure(i, weight=1)

button22 = tkinter.Button(frame6, image=None, command=lambda: button_click(2, 2))
button22.grid(row=0, column=0, sticky="nsew", padx=40, pady=40)
buttons[(2, 2)] = button22

button23 = tkinter.Button(frame6, image=None, command=lambda: button_click(2, 3))
button23.grid(row=0, column=1, sticky="nsew", padx=40, pady=40)
buttons[(2, 3)] = button23

button24 = tkinter.Button(frame6, image=None, command=lambda: button_click(2, 4))
button24.grid(row=1, column=0, sticky="nsew", padx=40, pady=40)
buttons[(2, 4)] = button24

button25 = tkinter.Button(frame6, image=None, command=lambda: button_click(2, 5))
button25.grid(row=1, column=1, sticky="nsew", padx=40, pady=40)
buttons[(2, 5)] = button25

#frame7

frame7 = tkinter.Frame(root, bd=10, relief='ridge')
frame7.grid(row=2, column=3, rowspan=1, columnspan=2, sticky="nsew")

for i in range(2):
    frame7.columnconfigure(i, weight=1)
frame7.rowconfigure(0, weight=1)

button26 = tkinter.Button(frame7, image=None, command=lambda: button_click(2, 3))
button26.grid(row=0, column=0, sticky="nsew", padx=40, pady=40)
buttons[(2, 3)] = button26

button27 = tkinter.Button(frame7, image=None, command=lambda: button_click(2, 4))
button27.grid(row=0, column=1, sticky="nsew", padx=40, pady=40)
buttons[(2, 4)] = button27

#frame8

frame8 = tkinter.Frame(root, bd=10, relief='ridge')
frame8.grid(row=3, column=3, rowspan=2, columnspan=3, sticky="nsew")
for i in range(2):
    frame8.rowconfigure(i, weight=1)
    frame8.columnconfigure(i, weight=1)
frame8.columnconfigure(2, weight=1)

button28 = tkinter.Button(frame8, image=None, command=lambda: button_click(3, 0))
button28.grid(row=0, column=0, sticky="nsew", padx=40, pady=40)
buttons[(3, 0)] = button28

button29 = tkinter.Button(frame8, image=None, command=lambda: button_click(3, 1))
button29.grid(row=0, column=1, sticky="nsew", padx=40, pady=40)
buttons[(3, 1)] = button29

button30 = tkinter.Button(frame8, image=None, command=lambda: button_click(3, 2))
button30.grid(row=0, column=2, sticky="nsew", padx=40, pady=40)
buttons[(3, 2)] = button30

button31 = tkinter.Button(frame8, image=None, command=lambda: button_click(3, 3))
button31.grid(row=1, column=0, sticky="nsew", padx=40, pady=40)
buttons[(3, 3)] = button31

button32 = tkinter.Button(frame8, image=None, command=lambda: button_click(3, 4))
button32.grid(row=1, column=1, sticky="nsew", padx=40, pady=40)
buttons[(3, 4)] = button32

button33 = tkinter.Button(frame8, image=None, command=lambda: button_click(3, 5))
button33.grid(row=1, column=2, sticky="nsew", padx=40, pady=40)
buttons[(3, 5)] = button33

#frame9

frame9 = tkinter.Frame(root, bd=10, relief='ridge')
frame9.grid(row=3, column=6, rowspan=1, columnspan=2, sticky="nsew")
for i in range(2):
    frame9.columnconfigure(i, weight=1)
frame9.rowconfigure(0, weight=1)

button34 = tkinter.Button(frame9, image=None, command=lambda: button_click(3, 6))
button34.grid(row=0, column=0, sticky="nsew", padx=40, pady=40)
buttons[(3, 6)] = button34

button35 = tkinter.Button(frame9, image=None, command=lambda: button_click(3, 7))
button35.grid(row=0, column=1, sticky="nsew", padx=40, pady=40)
buttons[(3, 7)] = button35

#frame10

frame10 = tkinter.Frame(root, bd=10, relief='ridge')
frame10.grid(row=4, column=0, rowspan=1, columnspan=3, sticky="nsew")

for i in range(3):
    frame10.columnconfigure(i, weight=1)
frame10.rowconfigure(0, weight=1)

button36 = tkinter.Button(frame10, image=None, command=lambda: button_click(4, 0))
button36.grid(row=0, column=0, sticky="nsew", padx=40, pady=40)
buttons[(4, 0)] = button36

button37 = tkinter.Button(frame10, image=None, command=lambda: button_click(4, 1))
button37.grid(row=0, column=1, sticky="nsew", padx=40, pady=40)
buttons[(4, 1)] = button37

button38 = tkinter.Button(frame10, image=None, command=lambda: button_click(4, 2))
button38.grid(row=0, column=2, sticky="nsew", padx=40, pady=40)
buttons[(4, 2)] = button38

#frame11

frame11 = tkinter.Frame(root, bd=10, relief='ridge')
frame11.grid(row=4, column=6, rowspan=2, columnspan=2, sticky="nsew")
for i in range(2):
    frame11.rowconfigure(i, weight=1)
    frame11.columnconfigure(i, weight=1)

button39 = tkinter.Button(frame11, image=None, command=lambda: button_click(4, 6))
button39.grid(row=0, column=0, sticky="nsew", padx=40, pady=40)
buttons[(4, 6)] = button39

button40 = tkinter.Button(frame11, image=None, command=lambda: button_click(4, 7))
button40.grid(row=0, column=1, sticky="nsew", padx=40, pady=40)
buttons[(4, 7)] = button40

button41 = tkinter.Button(frame11, image=None, command=lambda: button_click(5, 6))
button41.grid(row=1, column=0, sticky="nsew", padx=40, pady=40)
buttons[(5, 6)] = button41

button42 = tkinter.Button(frame11, image=None, command=lambda: button_click(5, 7))
button42.grid(row=1, column=1, sticky="nsew", padx=40, pady=40)
buttons[(5, 7)] = button42

#frame12

frame12 = tkinter.Frame(root, bd=10, relief='ridge')
frame12.grid(row=5, column=0, rowspan=3, columnspan=2, sticky="nsew")

for i in range(2):
    frame12.rowconfigure(i, weight=1)
    frame12.columnconfigure(i, weight=1)
frame12.rowconfigure(2, weight=1)

button43 = tkinter.Button(frame12, image=None, command=lambda: button_click(5, 2))
button43.grid(row=0, column=0, sticky="nsew", padx=40, pady=40)
buttons[(5, 2)] = button43

button44 = tkinter.Button(frame12, image=None, command=lambda: button_click(5, 3))
button44.grid(row=0, column=1, sticky="nsew", padx=40, pady=40)
buttons[(5, 3)] = button44

button45 = tkinter.Button(frame12, image=None, command=lambda: button_click(5, 4))
button45.grid(row=1, column=0, sticky="nsew", padx=40, pady=40)
buttons[(5, 4)] = button45

button46 = tkinter.Button(frame12, image=None, command=lambda: button_click(5, 5))
button46.grid(row=1, column=1, sticky="nsew", padx=40, pady=40)
buttons[(5, 5)] = button46

button47 = tkinter.Button(frame12, image=None, command=lambda: button_click(5, 6))
button47.grid(row=2, column=0, sticky="nsew", padx=40, pady=40)
buttons[(5, 6)] = button47

button48 = tkinter.Button(frame12, image=None, command=lambda: button_click(5, 7))
button48.grid(row=2, column=1, sticky="nsew", padx=40, pady=40)
buttons[(5, 7)] = button48

#frame13

frame13 = tkinter.Frame(root, bd=10, relief='ridge')
frame13.grid(row=5, column=2, rowspan=2, columnspan=1, sticky="nsew")

for i in range(2):
    frame13.rowconfigure(i, weight=1)
frame13.columnconfigure(0, weight=1)

button49 = tkinter.Button(frame13, image=None, command=lambda: button_click(6, 0))
button49.grid(row=0, column=0, sticky="nsew", padx=40, pady=40)
buttons[(6, 0)] = button49

button50 = tkinter.Button(frame13, image=None, command=lambda: button_click(6, 1))
button50.grid(row = 1, column = 0, sticky= "nsew", padx=40, pady=40)
buttons[(6, 1)] = button50

#frame14

frame14 = tkinter.Frame(root, bd=10, relief='ridge')
frame14.grid(row = 5, column = 3, rowspan = 2, columnspan = 2, sticky="nsew")

for i in range(2):
    frame14.rowconfigure(i, weight=1)
    frame14.columnconfigure(i, weight=1)

button51 = tkinter.Button(frame14, image=None, command=lambda: button_click(6, 2))
button51.grid(row = 0, column = 0, sticky= "nsew", padx=40, pady=40)
buttons[(6, 2)] = button51

button52 = tkinter.Button(frame14, image=None, command=lambda: button_click(6, 3))
button52.grid(row = 0, column = 1, sticky= "nsew", padx=40, pady=40)
buttons[(6, 3)] = button52

button53 = tkinter.Button(frame14, image=None, command=lambda: button_click(6, 4))
button53.grid(row = 1, column = 0, sticky= "nsew", padx=40, pady=40)
buttons[(6, 4)] = button53

button54 = tkinter.Button(frame14, image=None, command=lambda: button_click(6, 5))
button54.grid(row = 1, column = 1, sticky= "nsew", padx=40, pady=40)
buttons[(6, 5)] = button54

#frame15

frame15 = tkinter.Frame(root, bd=10, relief='ridge')
frame15.grid(row = 5, column = 5, rowspan = 3, columnspan = 1, sticky="nsew")

for i in range(3):
    frame15.rowconfigure(i, weight=1)
frame15.columnconfigure(0, weight=1)

button55 = tkinter.Button(frame15, image=None, command=lambda: button_click(6, 6))
button55.grid(row = 0, column = 0, sticky= "nsew", padx=40, pady=40)
buttons[(6, 6)] = button55

button56 = tkinter.Button(frame15, image=None, command=lambda: button_click(6, 7))
button56.grid(row = 1, column = 0, sticky= "nsew", padx=40, pady=40)
buttons[(6, 7)] = button56

button57 = tkinter.Button(frame15, image=None, command=lambda: button_click(7, 1))
button57.grid(row = 2, column = 0, sticky= "nsew", padx=40, pady=40)
buttons[(7, 1)] = button57

#frame16

frame16 = tkinter.Frame(root, bd=10, relief='ridge')
frame16.grid(row = 7, column = 2, rowspan = 1, columnspan = 3, sticky="nsew")

for i in range(3):
    frame16.columnconfigure(i, weight=1)
frame16.rowconfigure(0, weight=1)

button58 = tkinter.Button(frame16, image=None, command=lambda: button_click(7, 2))
button58.grid(row = 0, column = 0, sticky= "nsew", padx=40, pady=40)
buttons[(7, 2)] = button58

button59 = tkinter.Button(frame16, image=None, command=lambda: button_click(7, 3))
button59.grid(row = 0, column = 1, sticky= "nsew", padx=40, pady=40)
buttons[(7, 3)] = button59

button60 = tkinter.Button(frame16, image=None, command=lambda: button_click(7, 4))
button60.grid(row = 0, column = 2, sticky= "nsew", padx=40, pady=40)
buttons[(7, 4)] = button60

#frame17

frame17 = tkinter.Frame(root, bd=10, relief='ridge')
frame17.grid(row = 6, column = 6, rowspan = 2, columnspan = 2, sticky="nsew")

for i in range(2):
    frame17.rowconfigure(i, weight=1)
    frame17.columnconfigure(i, weight=1)

button61 = tkinter.Button(frame17, image=None, command=lambda: button_click(6, 6))
button61.grid(row = 0, column = 0, sticky= "nsew", padx=40, pady=40)
buttons[(6, 6)] = button61

button62 = tkinter.Button(frame17, image=None, command=lambda: button_click(6, 7))
button62.grid(row = 0, column = 1, sticky= "nsew", padx=40, pady=40)
buttons[(6, 7)] = button62

button63 = tkinter.Button(frame17, image=None, command=lambda: button_click(7, 6))
button63.grid(row = 1, column = 0, sticky= "nsew", padx=40, pady=40)
buttons[(7, 6)] = button63

button64 = tkinter.Button(frame17, image=None, command=lambda: button_click(7, 7))
button64.grid(row = 1, column = 1, sticky= "nsew", padx=40, pady=40)
buttons[(7, 7)] = button64

root.mainloop()
