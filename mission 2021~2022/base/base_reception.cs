/* Reception avec la base. */
using System;
using System.IO.Ports;

public class rover{

    public static void Main(string[] args){
        rover.connect();
    }
    
    public static void connect(){
        string[]ports = SerialPort.GetPortNames();
        for(int i = 0;i < ports.Lenght();i++){
            SerialPort.Open();
        }
    }
}
