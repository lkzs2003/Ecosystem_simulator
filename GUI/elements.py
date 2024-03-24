import os
import tkinter as tk
import matplotlib
matplotlib.use("TkAgg")
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.backends.backend_tkagg import NavigationToolbar2Tk
from matplotlib.figure import Figure
from tkinter import messagebox, simpledialog


    

class RoundBox(tk.Label):
    def __init__(self, text1,text2,width, height,arc, borderwidth,color, **kwargs):
        super().__init__()
        self.canvas = tk.Canvas( width=width+1, height=height+1)
        self.width = width
        self.height = height
        x = 2
        y = 2
        self.color = color
        c= arc
        self.canvas.create_arc(x,   y,   x+2*c,   y+2*c,   start= 90, extent=90, style="arc",outline=color,width=borderwidth)
        self.canvas.create_arc(x+width-2*c, y+height-2*c, x+width, y+height, start=270, extent=90, style="arc",outline=color,width=borderwidth)
        self.canvas.create_arc(x+width-2*c, y,   x+width, y+2*c,   start=  0, extent=90, style="arc",outline=color,width=borderwidth)
        self.canvas.create_arc(x,   y+height-2*c, x+2*c,   y+height, start=180, extent=90, style="arc",outline=color,width=borderwidth)
        self.canvas.create_line(x+c, y,   x+width-c, y,fill=color,width=borderwidth)
        self.canvas.create_line(x+c, y+height, x+width-c, y+height,fill=color,width=borderwidth )
        self.canvas.create_line(x,   y+c, x,     y+height-c,fill=color,width=borderwidth)
        self.canvas.create_line(x+width, y+c, x+width,   y+height-c, fill=color,width=borderwidth)
        self.label = tk.Label( text=text1, fg=self.color)
        self.label_dynamic = tk.Label( textvariable=text2)
        
        
    def place(self, x, y):
        
        self.canvas.place(x=x-self.width/4, y=y-5)
        self.label.place(x=x+10, y=y)
        self.label_dynamic.place(x=x+10, y=y+20)
    
    

        