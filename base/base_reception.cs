/* Reception avec la base. */
#define debug

using System;
using System.IO.Ports;

public class reception{
    public static void setup(){
        reception.detecter_et_connecter();
    }

    public static void loop(){
    }

    public static void detecter_et_connecter(){
        Console.WriteLine("Connection au port. ");
        string[]ports = SerialPort.GetPortNames();
        for(int i=0;i<ports.Lenght();i++){
            try{
                SerialPort.Open();
            }catch(UnauthorizedAccessException){
                Console.WriteLine("Erreur: l'accès au port "+ports[i]+" refusé.");
                Console.WriteLine("     Voir: docs.microsoft.com/fr-fr/dotnet/api/system.unauthorizedexception");
            }catch(ArgumentOutOfRangeException){
                Console.WriteLine("Erreur: une ou plusieurs propriétés ne sont pas valides. ");
                Console.WriteLine("     Voir: docs.microdoft.com/fr-fr/dotnet/api/system.argumentoutofrangeexception");
            }catch(ArgumentException){
                Console.WriteLine("Erreur: le nom du port ne commence pas par COM. ");
                Console.WriteLine("     Voir: docs.microsoft.com/fr-fr/dotnet/api/system.argumentexception");
            }catch(IOException){
                Console.WriteLine("Erreur: l'état du port n'est pas valide. ");
                Console.WriteLine("     Voir: docs.microsoft.com/fr-fr/dotnet/api/system.ioexception");
            }catch(InvalidOperationException){
                Console.WriteLine("Erreur: port déjà ouvert. ");
                Console.WriteLIne("     Voir: docs.microsoft.com/fr-fr/dotnet/api/system.invalidoperationexception");
            }
        }
#if debug
        Console.WriteLine("DEBUG: le port envoie:");
        Console.WriteLine(Console.ReadLine());
#endif
    }
}