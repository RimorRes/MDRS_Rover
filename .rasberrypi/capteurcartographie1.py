#Cartographie, fonctionne selon les Capteurs 

#----------------------------#
#----------------------------#
#----------------------------#
#----------------------------#
#----------------------------#
#----------------------------#
#TOUTE EST CONVERTIE EN MÈTRE#
#----------------------------#
#----------------------------#
#----------------------------#
#----------------------------#
#----------------------------#
#----------------------------#
 
#-Principe du programme'Cartographie'-
#On crée le plan xyz qui contiendra les informations nourris par les capteurs: 
#ce plan sera une collection de case se trouvant sur une axe xy et possiblement z.
#Dans chaque case se trouvera ces informations cités.
#
# À préciser : Mesure GPS : Latitude/longitude ?
#
# Format de DATA et GPS (argument) => (''Nom de la mesure etudie'' ,valeur) ; (latitude/longitude)
# Format de DATA et GPS (in csv)=> ;(Y,X)}Temperature=54}Pression=43} ...;
# Format de DATA et GPS (après readline)=> ['(Y,X)}Temperature=54}Pression=43} ...']
#
# Mesure enregistrer (dans l'ordre) => Temperature, Pression, Pression/Vitesse du vent,
# Orientation du vent , Magnetique , distance parcourue
#
#Cartographie se lance lorsque le GPS est en ligne et qu'on a décidé quelle 
#cordonnée du GPS sera attribué à caseOrigine.
#
#La première colonne du fichier csv ne pourra pas faire partie de la "carte". 
#C'est à dire que aucune valeur de capteur/gps ne pourra y être insérer, penser 
#comme si elle était une colonne qui va inclures les "légendes" de la "carte".
#Ces légendes consiste à dire si le fichier est vide, ou si il y a une caseorigine
#et dans ce cas d'afficher les coordonnées de cette caseorigine. Toute fois, cette caseorigine
#va quand même figuré sur la "carte" elle même. Sert strictement qu'à l'optimisation du code,
#même si j'ai conscient que par conséquence, ça va être une torture de travailer sur ce code.  
#
# Les cases vides, lorsque on balance la liste dans un fichier csv, seront représenter par 'None' (sv le souhaiter)
#
#Première colonne => 'Vide' ou "caseOrigine 3.47239473892=324.437578943 234.423=133=76.312=90.4123"
#
#xyz = [''(Y,X)}Temperature=T}Pression=P}PVDuVent=V}OrientationDuVent=O
#}Magnetique=M}DistanceParcourue=D'']
#
#locale_cartographie garde les coordonnées des 'murs' de la cartographie et de caseOrigine. Sert à optimiser le code 
#------Ordre de passage dans le code------:
#-xyz crée 'Le plan'.
#
#-On nourrie la fonction Spoonfeeding de donnée et de la localisation du GPS.Celui-ci va aussi organiser xyz 
#
#-On calcule la différence des coordonnées GPS X Y de l'emplacement actuel du  
#rover avec celui de la case crée en premier (caseOrigine) dans la fonction Spoonfeeding.
#
#-On insert dans le fichier csv à travers Quadrilatere, les données,coordonnées GPS  dans la 
#case ayant comme coordonnées la différence des coordonnées GPS de caseOrigine et de la
#case actuelle.
#
#-Fonction 'Maj' servira, si on a besoin de tel fonction, de mettre à jour les données
#----------------------------------------#

#Note
# Ordre GPS => Y , X
# Ordre coord murs => X1 Y1 X2 Y2
# Première colonne => 'Vide' ou "caseOrigine Y=X X1=Y1=X2=Y2"
# locale_cartographie => Ycaseorigine,Xcaseorigine X1 Y1 X2 Y2

from math import *
#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#
def Cartographie(DATA,GPS):
    
    locale_cartographie = False
    xyz_cartographie = []
    global MESURE_
    
    def translation(Coord,type_): #Prend en argument un coordonnée de GPS sous forme de degrée type_ => X ? Y ?
            # return Coord*111.32 , return (40075 * math.cos(Coord) / 360)
            # je n'en sais absoluemment rien À VOIR
    
    def Comparaison(GPS):#détermine si on doit changer les valeurs des coordonnées des murs
        x1 = locale_cartographie[2]
        y1 = locale_cartographie[3]
        x2 = locale_cartographie[4]
        y2 = locale_cartographie[5]
      
        if GPS[1] < x1:
            x1 = GPS[1]
        if GPS[0] < y1: 
            y1 =GPS[0]
        if GPS[1] > x2:
            x2 = GPS[1]
        if GPS[0] > y2:
            y2 = GPS[0]
        return x1,y1,x2,y2
        
    
    
    def Quadrilatere ():  
        #Sauvegarde xyz dans le fichier csv. Ne s'occupe pas de l'organisation de
        #la liste ou de convertir tout en string (tache faite par les autres fonctions)
        pass


    def Data_add(DATA,GPS,COORDY,COORDX): #Permet de modifier les valeurs des cases
        y = translation(locale_cartographie[0],'y')/MESURE_ - Y
        x = translation(locale_cartographie[1],'x')/MESURE_ - X
    

    def Post_zero(y1,y2,x1,x2): #Change le contenue de caseOrigine soit les coordonnées des murs
            Option=xyz_cartographie[0].split(' ')[2].split('=')
            Option[0]=x1  
            Option[1]=y1 
            Option[2]=x2 
            Option[3]=y2
            Option = str(Option[0]) + '=' + str(Option[1]) + '=' + str(Option[2]) + '=' + str(Option[3]) 
            xyz_cartographie[0] =  'caseOrigine' + ' ' + len(xyz_cartographie[0].split(' ')[1].split['='][0]) + '=' + len(xyz_cartographie[0].split(' ')[1].split['='][1]) + ' '+ Option
            return
    
    
    def Zero(GPS): #Remplit une première fois caseOrigine
            
            xyz_cartographie[0]='caseOrigine' + ' ' + str(GPS[0]) + '=' + str(GPS[1]) + ' ' + str(GPS[1]) + '=' + str(GPS[0]) + '=' + str(GPS[1]) + '=' + str(GPS[0])
            xyz_cartographie.append([])
            xyz_cartographie[1].append(Data_add(DATA,GPS))
            return
        

        
    def Spoonfeeding(DATA,GPS) : #Arrange la liste xyz
        #Partie où on détermine les coordonnées de la case qu'on veut ajouter dans la liste

        Y=0
        X=0
'''
        Y=translation(GPS[0],'y') - translation(locale_cartographie[0],'y')
        Y/=MESURE_
        X = translation(GPS[1],'x') - translation(locale_cartographie[1],'x')
        X/=MESURE_
'''
        Y = GPS[0] - locale_cartographie[0]
        Y = translation(Y,'y') / MESURE_
        X = GPS[1] - locale_cartographie[1]
        X = translation(X,'x') / MESURE_
        


        if abs(X)-abs(X)//1 >= 0.5:
            if X>0:
                X= X//1 + 1
            else:
                X = X//1 - 1
        else:
            X//=1                #Détermination des coordonnées de la case dans le fichier 
                                 #Expérimentale, enclin à  beuguer de L91 à L114
        if abs(Y)-abs(Y)//1 >= 0.5:
            if Y>0:
                Y= Y//1 + 1
            else:
                Y = Y//1 - 1
        else:
            Y//=1    
        
      #----------------------#  
      #Partie où on détermine les murs et leurs étend
        x1=0
        y1=0
        x2=0
        y2=0     
        x1 , y1 , x2 , y2 = Comparaison(GPS)
        Post_zero(x1,y1,x2,y2)
        
        x1 = x1 - locale_cartographie[2]
        y1 = y1 - locale_cartographie[3]
        x2 = x2 - locale_cartographie[4]
        y2 = y2 - locale_cartographie[5]
        
        x1 = translation(x1,'x') / MESURE_
        y1 = translation(x1,'y') / MESURE_
        x2 = translation(x2,'x') / MESURE_
        y2 = translation(y2,'y') / MESURE_
        
        if x1 < 0 :
            for Minus_X_VERT in range (1,len(xyz_cartographie)):
                for Minus_X_HOR in range(abs(x1)):
                    xyz_cartographie[Minus_X_VERT].insert(0,'None')
        if y1 < 0 :
            for Minus_Y_VERT in range (abs(y1)):
                CASE0deplace = xyz_cartographie[0] #Deplace caseOrigine dans la nouvelle ligne
                xyz_cartographie[0] = ['None' for i in range(len(xyz_cartographie[1]))]
                xyz_cartographie.insert(0,CASE0Ddeplace)
        
        if x2 > 0:
            for Max_X_VERT in range (1,len(xyz_cartographie)):
                for MAX_X_HOR in range(x2):
                    xyz_cartographie[Max_X_VERT].append('None')
        
        if y2 > 0:
            for Max_Y_VERT in range (y2):
                xyz_cartographie.append(['None' for i in range (len(xyz_cartographie[1]))])
        
        #----------------------#  
        #Partie où on ajoute les données, les coordonnées connues
        
        Data_add(DATA,GPS,Y,X)



    #--------------------------------------------------------------------------#
    #--------------------------------------------------------------------------#
    #--------------------------------------------------------------------------#
    #--------------------------------------------------------------------------#

    #Initialisation de XYZ et de caseOrigine
    #
    #locale_cartographie va retenir les coordonées GPS de caseOrigine, utile 
    #seulement dans cette fonction
    
    
    #2
    def chercher_zero(): #Chercher caseOrigine d'une cartographie
        if 'caseOrigine' in xyz_cartographie[0]:
            return xyz_cartographie[0]
        elif xyz_cartographie[0]=="Vide":
            return False
            
            
    #1
    def initialisation(GPS): #Créer le tableau xyz  + locale_cartographie
        file = open('cartographie.csv','r')     
        for a in file:
            xyz_cartographie.append(a.rstrip().split(';'))
        file.close()
            
        locale_cartographie=chercher_zero
    #3    
        if locale_cartographie==False: #Si le fichier est vide
            Zero(GPS)
            Quadrilatere()
            return False 
           
    #3    
        else : #Sinon commencer a la faire remplir
            locale_cartographie = locale_cartographie.split(' ')
            tranducteur_cartographie =locale_cartographie.split('=')
            locale_cartographie,rajouter_coord_mur = [float(traducteur_cartographie[0]),float(traducteur_cartographie[1])] , locale_cartographie.split(' ')[2]
            
            for boucle_coord_mur in rajouter_coord_mur:
                locale_cartographie.append(float(boucle_coord_mur))
            del(traducteur_cartographie)
            return True
    #--------------------------------------------------------------------------#
    #--------------------------------------------------------------------------#
    #--------------------------------------------------------------------------#
    #--------------------------------------------------------------------------#
    #4
  
    resultat = initialisation(GPS)
    if resultat: #Si il existe une caseOrigine, alors à partir de son coordonnées entrer les valeurs
        Spoonfeeding(DATA,GPS)
        Quadrilatere()
    return 

#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#
#------------------------------------------------------------------------------#









