# Bradley Elenbaas
# Terrortronics
# mr.elenbaas@gmail.com


import socket
import subprocess
import tkinter


TEXT_HELLO_WORLD = 'HELLO WORLD'
TEXT_QUIT = 'QUIT'
TEXT_IPCONFIG = 'IPCONFIG'
TEXT_TRANSMIT_PACKET = 'TRANSMIT UDP'
PACK_SIDE = 'top'
CALL_IP_CONFIG = 'ipconfig'
IP_ADDRESS = '192.168.0.1'
PORT_NUMBER = 50007
PACKET_DATA = 'Hello!'.encode()


class GUI(tkinter.Frame):
    """
    Tkinter GUI.
    """
    
    def __init__(self):
        """
        Setup Tkinter & buttons.
        """
        
        self.__root = tkinter.Tk()
        
        super().__init__(self.__root)
        
        self.pack()
        tkinter.Button(
            self,
            text = TEXT_HELLO_WORLD,
            command = self.__print_hello_world
        ).pack(side = PACK_SIDE)
        tkinter.Button(
            self,
            text = TEXT_IPCONFIG,
            command = self.__call_ip_config
            ).pack(side = PACK_SIDE)
        tkinter.Button(
            self,
            text = TEXT_TRANSMIT_PACKET,
            command = self.__transmit_packet
        ).pack(side = PACK_SIDE)
        tkinter.Button(
            self,
            text = TEXT_QUIT,
            command = self.__root.destroy
            
        ).pack(side = PACK_SIDE)
        self.mainloop()
    
    def __call_ip_config(self):
        """
        """
        
        subprocess.call([CALL_IP_CONFIG])
    
    def __print_hello_world(self):
        """
        Just print "Hello World" for now.
        """
        
        print(TEXT_HELLO_WORLD)
    
    def __transmit_packet(self):
        """
        """
        
        s = socket.socket(
            socket.AF_INET,
            socket.SOCK_DGRAM,
            0
        )
        s.connect(
            (IP_ADDRESS, PORT_NUMBER)
        )
        s.send(
            PACKET_DATA
        )
        s.close()
        

if __name__ == "__main__":
    """
    Main loop.
    """
    
    GUI()
    
    print('complete')
