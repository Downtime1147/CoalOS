import setuptools, os, sys, time, winsound, random, pygame
from datetime import datetime

#-------------------------------------------------------------------------------------------------------------------------------

def load(Dir): # fuction to load text from a .txt file
    with open(Dir, 'r', encoding = 'utf8') as screen:
        for lines in screen:
            print(lines, end = '')

#-------------------------------------------------------------------------------------------------------------------------------

def bar(it, prefix="", size=60, file=sys.stdout): # function that creates a progress bar
    count = len(it)
    def show(j):
        x = int(size*j/count)
        file.write("%s[%s%s] %i/%i\r" % (prefix, "#"*x, "="*(size-x), j, count))
        file.flush()        
    show(0)
    for i, item in enumerate(it):
        yield item
        show(i+1)
    file.write("\n")
    file.flush()

#-------------------------------------------------------------------------------------------------------------------------------

def uInput(): # user input
    global cmd

    uInput = input("root > ")
    cmd = uInput.lower()

    if cmd == 'clear':
        Clear()

    if cmd == 'restart':
        Restart()

    if cmd == 'help':
        Help()

    if cmd == 'iwlist':
        Scan()

    if cmd == 'aireplay':
        airPlay()

    if cmd == 'aircon':
        airConnect()

    if cmd == 'ftpea':
        FTpea()

    if cmd == 'logout':
        Logout()

    if cmd == 'news':
        News()

    if cmd == 'cal':
        Cal()

    if cmd == 'nmap':
        Nmap()

    if cmd == 'sscrack':
        SScrack()

    else:
        Error()

#-------------------------------------------------------------------------------------------------------------------------------

def Clear(): # clears screen
    os.system('cls' if os.name == 'nt' else 'clear')
    uInput()

#-------------------------------------------------------------------------------------------------------------------------------

def Restart(): # Does exactly as you'd think
    os.system('cls' if os.name == 'nt' else 'clear')
    time.sleep(1.5)
    boot()

#-------------------------------------------------------------------------------------------------------------------------------

def Cal(): # Displays real date and time
    now = datetime.now()
    print()
    print(now.day, now.month, now.year, "/", now.hour, ":", now.minute)
    print()
    uInput()

#-------------------------------------------------------------------------------------------------------------------------------

def News(): # Picks random headline from file
    with open("Dependencies/Tools/headlines.txt") as f:
        lines = f.readlines()
        print()
        print(random.choice(lines))
        print()

    uInput()

#-------------------------------------------------------------------------------------------------------------------------------

def SScrack(): # For 'cracking' ports that are vulnerable to SSH attacks
    print()
    load('Dependencies/Graphics/sscrack.txt')
    print()
    print()
    ssInput()

def ssInput():
    SSH = input("root ~ SScrack > ")
    sscrack = SSH.lower()

    if sscrack == 'help':
        ssHelp()
    
    if sscrack == 'exit':
        print()
        print()
        uInput()

    if sscrack == 'porthack':
        portHack()

    else:
        ssError()

#-----------------------------------------------------------

def ssError():
    print()
    print("That is not a valid SScrack command. Type help to view valid commands")
    print()
    winsound.PlaySound("Dependencies/Sound/error.wav", winsound.SND_FILENAME)
    winsound.PlaySound("Dependencies/Sound/idle.wav", winsound.SND_LOOP|winsound.SND_ASYNC)
    ssInput()

#-----------------------------------------------------------

def portHack():
    global portIP
    global portmac
    global portnum
    
    print()

    portIP = input("root ~ SScrack > Enter IP Address : ")
    portmac = input("root ~ SScrack > Enter a valid MAC Address : ")
    portnum = input("root ~ SScrack > Enter a valid Port Number : ")

    if portIP == '177.93.84.162.14':
        if portmac == '6g:11:hf:9q:mb:pi:97:2i' or 'y3:91:4r:gt:ue:33:7o:p1' or '00:r1:o8:v7:nq:73:55:o8':
            if portnum == '1' or '2' or '3':
                ssMain()
            
            else:
                print()
                print("root ~ SScrack > Portnumber ", portnum, " is invalid, is protected againts this attack, or already open...")
                print()
                ssInput()

        else:
            print()
            print("root ~ SScrack > PortMAC ", portmac, " is invalid, is protected againts this attack, or already open...")
            print()
            ssInput()
            
    else:
        print()
        print("root ~ SScrack > PortIP ", portIP, " is invalid, is protected againts this attack, or already open...")
        print()
        ssInput()

#-----------------------------------------------------------

def ssMain(): # main body of the portHack() function
    print()
    print("root ~ SScrack > Starting SSH attack against MAC Address : ", portmac, " ( Port IP : ", portIP, " ) ...")
    time.sleep(3.5)
    print()
    print("root ~ SScrack > Accessing client...")
    time.sleep(1.6)
    print()
    print("root ~ SScrack > Locating encryption key...")
    time.sleep(3)
    print()
    print("root ~ SScrack > Locating encryption key (46%)...")
    time.sleep(5)
    print()
    print("root ~ SScrack > Locating encryption key (79%)...")
    time.sleep(3.5)
    print()
    print("root ~ SScrack > Encryption key located  :)")
    time.sleep(1)
    print()
    print("root ~ SScrack > Decrypting data...")
    time.sleep(3)
    print()
    print("root ~ SScrack > Decrypting data (28%)...")
    time.sleep(4.5)
    print()
    print("root ~ SScrack > Decrypting data (46%)...")
    time.sleep(5)
    print()
    print("root ~ SScrack > Decrypting data (67%)...")
    time.sleep(2.4)
    print()
    print("root ~ SScrack > Decrypting data (82%)...")
    time.sleep(3.5)
    print()
    print("root ~ SScrack > Decrypting data (97%)...")
    time.sleep(2)
    print()
    print("root ~ SScrack > Data Decrypted  :)")
    time.sleep(1.5)
    print()
    print("root ~ SScrack > Port Unlocked !")
    time.sleep(0.6)

    if portIP == '177.93.84.162.14':
        print()
        print("Password found: Discrom77681E4QR!")
        print()
    
    ssInput()

#-----------------------------------------------------------

def ssHelp():
    print()
    print("---------------------------------------------------------")
    print()
    print('porthack - cracks SSH vulnerable ports')
    print()
    print("exit - disconnect from the SScrack client")
    print()
    print("sshelp - displays this help list")
    print()
    print("---------------------------------------------------------")
    print()
    ssInput()

#-------------------------------------------------------------------------------------------------------------------------------

def Logout():
    print()
    print("Are you sure?  Y/n")
    print()
    logout = input("> ")
    sure = logout.lower()

    if sure == "y":
        print()
        print("Goodbye...")
        winsound.PlaySound("Dependencies/Sound/OldWindows(Shutdown)", winsound.SND_FILENAME)
        print()
        sys.exit()
    if sure == "n":
        print()
        uInput()

#-------------------------------------------------------------------------------------------------------------------------------

def FTpea():
    print()
    load('Dependencies/Graphics/ftpea.txt')
    print()
    print()
    ftInput()

#------------------------------------------------

def ftInput():
    FTP = input("root ~ FTpea > ")
    ftpea = FTP.lower()

    if ftpea == 'exit':
        print()
        print()
        uInput()

    if ftpea == 'help':
        ftHelp()

    if ftpea == 'overflow':
        ftHack()

    else:
        ftError()

#------------------------------------------------

def ftHelp():
    print()
    print("---------------------------------------------------")
    print()
    print("exit : exit the FTpea client")
    print("fthelp : pea-rints this help statement")
    print('overflow : overflow port with "peas"')
    print()
    print("---------------------------------------------------")
    print()
    ftInput()

#------------------------------------------------

def ftHack():
    global ftIP
    global ftMAC
    global ftport

    print()

    ftIP = input("root ~ FTpea > Enter IP Address : ")
    ftMAC = input("root ~ FTpea > Enter a valid MAC Address : ")
    ftport = input("root ~ FTpea > Enter a valid port : ")

    if ftIP == '31.638.48.35.744':
        if ftMAC == 'lr:5t:io:11:7e:p0:jy:1t' or '44:te:n7:mp:1q:re:b4:99' or 'cc:45:h7:9b:ar:oo:u7:76' or 'zx:12:5r:yy:pi:yn:44:11':
            if ftport == '1' or '2' or '3' or '4':
                Overflow()

            else:
                print()
                print("root ~ FTpea > The port number, ", ftport, " is invalid, is protected againts this attack, or already open...")
                print()
                ftInput()

        else:
            print()
            print("root ~ FTpea > The MAC Address, ", ftMAC, " is invalid, is protected againts this attack, or already open...")
            print()
            ftInput()

    else:
        print()
        print("root ~ FTpea > The IP Address, ", ftIP, " is invalid, is protected againts this attack, or already open...")
        print()
        ftInput()

#------------------------------------------------

def Overflow():
    os.system('cls' if os.name == 'nt' else 'clear')

    load('Dependencies/Graphics/ftpea.txt')
    print()
    print()
    print("Begining FTP overflow attack on client ", ftIP, " ( MAC Address : ", ftMAC, " ) ...")
    time.sleep(4)
    print()

    for i in bar(range(25), 'Gathering proccessing power : ', 20): # progress bar usage
        time.sleep(0.21)
        os.system('cls' if os.name == 'nt' else 'clear')
        load('Dependencies/Graphics/ftpea.txt')
        print()
        print()
        print("Begining FTP overflow attack on client ", ftIP, " ( MAC Address : ", ftMAC, " ) ...")
        print()

    print()
    print("Breaching firewall ...")
    time.sleep(3.5)
    print()
    print("Locating servers ...")
    time.sleep(4.8)
    print()
    print("Servers located !")
    time.sleep(1)
    print()
    print("Directing all packet flow to ", ftMAC, "...")
    time.sleep(6.5)
    print()
    print("High data trafic on, ", ftIP, " detected ...")
    time.sleep(3.5)
    print()
    print("Overflow successful, port unlocked !")
    time.sleep(0.5)

    if ftIP == '31.638.48.35.744':
        print()
        print("Password found: tunnelTrain2pltfrm6?!")
        print()
    
    ftInput()

#------------------------------------------------

def ftError():
    print()
    print("root ~ FTpea > That was not a valid FTpea command, type help for a list of commands")
    print()
    winsound.PlaySound("Dependencies/Sound/error.wav", winsound.SND_FILENAME)
    winsound.PlaySound("Dependencies/Sound/idle.wav", winsound.SND_LOOP|winsound.SND_ASYNC)
    ftInput()

#-------------------------------------------------------------------------------------------------------------------------------

def Error(): # call when unexpected input is detected
    print()
    print("Command invalid. Type 'help' for a list of commands...")
    print()
    winsound.PlaySound("Dependencies/Sound/error.wav", winsound.SND_FILENAME)
    winsound.PlaySound("Dependencies/Sound/idle.wav", winsound.SND_LOOP|winsound.SND_ASYNC)
    uInput()

#-------------------------------------------------------------------------------------------------------------------------------

def Help():
    print()
    print("List of commands:")
    print()
    print("     aireplay - cracks 'open' wireless connections")
    print()
    print("     aircon - connects to an open wireless connection")
    print()
    print("     cal - prints the current date and time")
    print()
    print("     clear - clears the screen")
    print()
    print("     FTpea - Cracks ports that are vulnerable to FTP hacks")
    print()
    print("     help - displays list of commands")
    print()
    print("     iwlist - scans for, and displays nearby wireless internet connections")
    print()
    print("     logout - exit coalOS")
    print()
    print("     news - display the latest news reports from all over")
    print()
    print("     nmap - scans input IP for available ports")
    print()
    print("     restart - restarts CoalOS")
    print()
    print("     SScrack - Cracks ports that are vulnerable to SSH hacks")
    print()
    uInput()

#-------------------------------------------------------------------------------------------------------------------------------

def Scan(): # prints 'iwlist.txt'
    print()
    print("Scanning...")
    time.sleep(2.5)
    print("7 wireless connections found...")
    time.sleep(0.6)
    print()
    load('Dependencies/Tools/iwlist.txt')
    print()
    print()
    uInput()

#-------------------------------------------------------------------------------------------------------------------------------

def airConnect(): # login to random connections here
    global log1
    global log2
    print()
    
    log1 = input("root ~ Enter IP Address : ")
    log2 = input("root ~ Enter Password : ")

    if log1 == '735.92.44.64.846':
        if log2 == 'mnQwertrewQnm123':
            print()
            print("Attempting login...")
            winsound.PlaySound("Dependencies/Sound/modem.wav", winsound.SND_FILENAME)
            winsound.PlaySound("Dependencies/Sound/idle.wav", winsound.SND_LOOP|winsound.SND_ASYNC)
            print()
            print("Login successful!")
            print()
            Device()

    if log1 == '653.67.49.847.46':
        if log2 == 'MmkwgUltbara98':
            print()
            print("Attempting login...")
            winsound.PlaySound("Dependencies/Sound/modem.wav", winsound.SND_FILENAME)
            winsound.PlaySound("Dependencies/Sound/idle.wav", winsound.SND_LOOP|winsound.SND_ASYNC)
            print()
            print("Login successful!")
            print()
            Device()

    if log1 == '273.84.845.53.32':
        if log2 == 'ddid.No8817994':
            print()
            print("Attempting login...")
            winsound.PlaySound("Dependencies/Sound/modem.wav", winsound.SND_FILENAME)
            winsound.PlaySound("Dependencies/Sound/idle.wav", winsound.SND_LOOP|winsound.SND_ASYNC)
            print()
            print("Login successful!")
            print()
            Device()

    if log1 == '177.93.84.162.14':
        if log2 == 'Discrom77681E4QR!':
            print()
            print("Attempting login...")
            winsound.PlaySound("Dependencies/Sound/modem.wav", winsound.SND_FILENAME)
            winsound.PlaySound("Dependencies/Sound/idle.wav", winsound.SND_LOOP|winsound.SND_ASYNC)
            print()
            print("Login successful!")
            print()
            Device()

    if log1 == '31.638.48.35.744':
        if log2 == 'tunnelTrain2pltfrm6?!':
            print()
            print("Attempting login...")
            winsound.PlaySound("Dependencies/Sound/modem.wav", winsound.SND_FILENAME)
            winsound.PlaySound("Dependencies/Sound/idle.wav", winsound.SND_LOOP|winsound.SND_ASYNC)
            print()
            print("Login successful!")
            print()
            Device()

    if log1 == '98.712.345.63.28':
        if log2 == 'AllTheGames911711':
            print()
            print("Attempting login...")
            winsound.PlaySound("Dependencies/Sound/modem.wav", winsound.SND_FILENAME)
            winsound.PlaySound("Dependencies/Sound/idle.wav", winsound.SND_LOOP|winsound.SND_ASYNC)
            print()
            print("Login successful!")
            print()
            Device()

        else:
            print()
            print("Password,", log2, "is invalid...")
            print()
            uInput()
    else:
        print()
        print("IP address,", log1, "is invalid...")
        print()
        uInput()

#-------------------------------------------------------------------------------------------------------------------------------

def airPlay(): # 'finds' passwords
    global entry1
    global entry2
    
    print()
    entry1 = input("root ~ Enter IP Address : ")
    entry2 = input("root ~ Enter Essid : ")

    if entry1 == '735.92.44.64.846':
        if entry2 == 'Zx@P!o33Q7':
            crack()

    if entry1 == '653.67.49.847.46':
        if entry2 == 'Hals_Comp':
            crack()

    if entry1 == '273.84.845.53.32':
        if entry2 == 'NVB_271324':
            crack()

    if entry1 == '98.712.345.63.28':
        if entry2 == 'RF8_11G4M3':
            crack()
            
        else:
            print()
            print("Essid ", entry2, " not found...")
            print()
            uInput()
    else:
        print()
        print("IP address ", entry1, " not found...")
        print()
        uInput()

#-------------------------------------------------------------------------------------------------------------------------------

def Device():
    print()

    if log2 == 'mnQwertrewQnm123':
        print("-----------------------------------------------------------")
        print("  Device IP : 735.92.44.64.846 | Device Name : Zx@P!o33Q7")
        print("-----------------------------------------------------------")
        devInput()

    if log2 == 'MmkwgUltbara98':
        print("-----------------------------------------------------------")
        print("  Device IP : 653.67.49.847.46 | Device Name : Hal's_Comp")
        print("-----------------------------------------------------------")
        devInput()

    if log2 == 'ddid.No8817994':
        print("-----------------------------------------------------------")
        print("  Device IP : 273.84.845.53.32 | Device Name : NVB_271324")
        print("-----------------------------------------------------------")
        devInput()

    if log2 == 'Discrom77681E4QR!':
        print("-----------------------------------------------------------")
        print("  Device IP : 177.93.84.162.14 | Device Name : VB0SE_CORP")
        print("-----------------------------------------------------------")
        devInput()

    if log2 == 'tunnelTrain2pltfrm6?!':
        print("-----------------------------------------------------------")
        print("  Device IP : 31.638.48.35.744 | Device Name : DESKNo.747")
        print("-----------------------------------------------------------")
        devInput()

    if log2 == 'AllTheGames911711':
        print("-----------------------------------------------------------")
        print("  Device IP : 98.712.345.63.28 | Device Name : RF8_11G4M3")
        print("-----------------------------------------------------------")
        devInput()

#---------------------------------------------------------------------------------

def devInput():
    if log2 == 'mnQwertrewQnm123':
        inPut = "root ~ 735.92.44.64.846 > "

    if log2 == 'MmkwgUltbara98':
        inPut = "root ~ 653.67.49.847.46 > "

    if log2 == 'ddid.No8817994':
        inPut = "root ~ 273.84.845.53.32 > "

    if log2 == 'Discrom77681E4QR!':
        inPut = "root ~ 177.93.84.162.14 > "

    if log2 == 'tunnelTrain2pltfrm6?!':
        inPut = "root ~ 31.638.48.35.744 > "

    if log2 == 'AllTheGames911711':
        inPut = "root ~ 98.712.345.63.28 > "
    
    device = input(inPut)
    devcmd = device.lower()

    if devcmd == 'exit':
        print()
        print("Disconnecting from ", log1, " ...")
        time.sleep(2)
        print()
        uInput()

    if devcmd == 'help':
        devHelp()

    if devcmd == 'list':
        SysList()

    if devcmd == 'clear':
        devClear()

    if devcmd == 'snake':
        if log2 == 'AllTheGames911711':
            Snakes()
        else:
            devError()

    else:
        devError()

#--------------------------------------------------

def devError():
    print()
    print("Invalid remote command, type 'help' for a list of commands...")
    winsound.PlaySound("Dependencies/Sound/error.wav", winsound.SND_FILENAME)
    winsound.PlaySound("Dependencies/Sound/idle.wav", winsound.SND_FILENAME|winsound.SND_ASYNC)
    print()
    devInput()

#--------------------------------------------------

def devHelp():
    print()
    print("-----------------------------------------------------------------------------------------")
    print("clear - clears the screen")
    print("exit - disconnect from current connection")
    print("help - displays this screen")
    print("list - lists the whole filesystem")
    print("-----------------------------------------------------------------------------------------")
    print()
    devInput()

#-----------------------------------------------------------

def devClear():
    os.system('cls' if os.name == 'nt' else 'clear')
    Device()

#-----------------------------------------------------------

def SysList():
    print()

    if log2 == 'mnQwertrewQnm123':
        load('Dependencies/Tools/SysList/device01.txt')
        print()
        print()
        devInput()

    if log2 == 'MmkwgUltbara98':
        load('Dependencies/Tools/SysList/device02.txt')
        print()
        print()
        devInput()

    if log2 == 'ddid.No8817994':
        load("Dependencies/Tools/SysList/device03.txt")
        print()
        print()
        devInput()

    if log2 == 'Discrom77681E4QR!':
        load("Dependencies/Tools/SysList/device04.txt")
        print()
        print()
        devInput()

    if log2 == 'tunnelTrain2pltfrm6?!':
        load("Dependencies/Tools/SysList/device05.txt")
        print()
        print()
        devInput()

    if log2 == 'AllTheGames911711':
        load("Dependencies/Tools/SysList/device06.txt")
        print()
        print()
        devInput()

#-----------------------------------------------------------------

def Snakes():
    os.system('python Dependencies/Tools/Snake.py')
    print()
    devInput()

#-------------------------------------------------------------------------------------------------------------------------------

def Nmap(): # 'scans' for available 'ports' in local network
    print()
    load('Dependencies/Graphics/nmap.txt')
    print()
    print()
    
    map1 = input("root ~ Enter IP Address : ")
    print()
    print("Scanning...")
    time.sleep(1.3)

    if map1 == '177.93.84.162.14':
        print()
        load('Dependencies/Tools/Nmap/nmap1.txt')
        print()
        print()
        uInput()

    if map1 == '31.638.48.35.744':
        print()
        load('Dependencies/Tools/Nmap/nmap2.txt')
        print()
        print()
        uInput()

    else:
        print()
        print("IP Address ", map1, " not found...")
        print()
        uInput()

#-------------------------------------------------------------------------------------------------------------------------------

def crack(): # main body of the airPlay() function...
    print()
    print("Starting attack against", entry2, "(IP address :", entry1,")")
    time.sleep(4.5)
    print()
    print("acquiring handshakes...")
    time.sleep(3)
    print("acquiring handshakes (25%)...")
    time.sleep(1.5)
    print("acquiring handshakes (50%)...")
    time.sleep(2)
    print("acquiring handshakes (75%)...")
    time.sleep(2.5)
    print("acquiring handshakes (100%)...")
    time.sleep(0.6)
    print()
    print("Overview of captured handshakes:")
    print()
    time.sleep(1)
    print("Captured handshake from '",entry1,"' : Contained a valid handshake : File saved to list 'redy2crack'...")
    time.sleep(0.6)
    print("Captured handshake from '",entry2,"' : Contained a valid handshake that matches that of '",entry1,"'...")
    time.sleep(2.5)
    print()
    print("Attempting password crack from dictionary, 'probable'...")
    time.sleep(5)
    print("Password crack failed...")
    time.sleep(3)
    print()
    print("Attempting password crack from dictionary, 'backup'...")
    time.sleep(7)
    print()
    print("Password crack successful...")
    time.sleep(1)

    if entry2 == 'Zx@P!o33Q7':
        print()
        print("Password is : mnQwertrewQnm123")
        print()

    if entry2 == 'Halls_Comp':
        print()
        print("Password is : MmkwgUltbara98")
        print()

    if entry2 == 'NVB_271324':
        print()
        print("Password is : ddid.No8817994")
        print()

    if entry2 == 'RF8_11G4M3':
        print()
        print("Password is : AllTheGames911711")
        print()
    
    uInput()
        
#-------------------------------------------------------------------------------------------------------------------------------


def boot(): # A cool startup cinematic
    os.system('cls' if os.name == 'nt' else 'clear')
    winsound.PlaySound("Dependencies/Sound/warmup.wav", winsound.SND_FILENAME|winsound.SND_ASYNC)
    load('Dependencies/Graphics/boot.txt')
    print()
    print()

    time.sleep(0.4)
    print("Calculated RAMsize: 66816233991524 Mb")
    time.sleep(0.2)
    print()
    print('BIOS load sequence :')
    print('     stepfunction(), load_clientend.jss...')
    print("     setvar.kernel/serverside.IPbinder.doccer_iv/447159098.tic")
    time.sleep(0.3)
    print("     query-88gheerq:latest.timedata;myname - kerneldriver.reqest_update...")
    time.sleep(0.2)
    print("     get-update.88611234637441010040083769393-OSpatch.xss")
    print("     apt-frombinaryConverter-ver.5.7.1/import * ...")
    print("     sudo-download arq-rootaccess/account88611 ...")
    time.sleep(0.2)
    print("     root access obtained!")
    time.sleep(0.1)
    print()
    print("     locate-manic\[yy67j-bio_reqesit-pack66]_update*()")
    print("     VssC++//C:1148-PSERRCODE#:55437889 / PKGNOTFOUND !")
    print("     Preparing issue fix :")
    time.sleep(0.2)
    print("          - Restart xx00001288447859000.tic")
    print("          - pkgFF0 rebootemulator.vdc-nowfunctional()")
    time.sleep(0.2)
    print("     - The following proccess will restart : xx00001288447859000.tic, serverside.IPbinder.doccer_iv/881343901.jsx")
    time.sleep(0.1)
    print("          RESTARTING ...")
    time.sleep(0.3)
    print("          reboot complete()")
    time.sleep(0.1)
    print()
    print("     scanning for error No. VssC++//C:1148-PSERRCODE#:55437889 ...")
    time.sleep(0.3)
    print("     PKG VssC++//C:1148-PSERRCODE#:55437889 successfully imported !")
    time.sleep(0.2)
    print()
    print("Scanning binarys:")
    time.sleep(0.2)
    print()
    for i in range(0,4):
        print("x 0101010101111011111010101 x 1001011000011001000100 x 101011111111011111000111101011111101111 xx 011001")
        print("01010100100000011111111 x 011001101 xx 1001010101000000111111101110101111000101111111110111010011101")
        time.sleep(0.1)
        print("010100001111111111111100111000011111010001111 x 0101000111111111011010000011110101110101111100101 x 001")
        print("0111110000000001 x 1101110111110101011111111101101001011 xx 10011110000101010000010101000011 x 111110101")
        print("11111111111111011111 x 11111101101111111111000000111111110110101 x 011110101010111111010111010101 x 0111")
        time.sleep(0.1)
        print("00001010101 xx 0100011001100110101 x 01010101010001111111101 x 010101011111010111 x 0100101010101101101001")
        time.sleep(0.1)
        print("x 0101010101111011111010101 x 1001011000011001000100 x 101011111111011111000111101011111101111 xx 011001")
        print("01010100100000011111111 x 01100 x 11 x 01 xx 1001010101000000111111101110101111000101111111110111010011101")
        time.sleep(0.2)
        print("11111101010101111111101101010000111110000111001101111000110101011 xx 10100111111011011 x 0110110100111")
        print("010100001111111111111100111000011111010001111 x 0101000111111111011010000011110101110101111100101001")
        time.sleep(0.1)
        print("00001010 x 00001111111 x 1111111100000 x 101001010100001111000001010010 x 10100000001011100000 x 000101")
    time.sleep(0.2)
    print()
    print("Scan complete: No errors")
    time.sleep(0.1)
    print()
    print("     Importing aditional dependancies and utils:")
    print("          - FTpea ver.8.4.6 ...")
    time.sleep(0.2)
    print("          - SScrack ver.2.1.3 ...")
    time.sleep(0.1)
    print("     <o>  - Nmap ver.8.9.1")
    time.sleep(0.1)
    print("          - sysOther ver.7.2.2 ")
    time.sleep(0.2)
    print("          - FireKernel ver 5.8.9")
    time.sleep(0.2)
    print("          - BinaryConverter ver 5.7.1")
    time.sleep(0.1)
    print()
    print("     running diagnostic test ...")
    time.sleep(0.6)
    print()
    print()
    print("BIOS loader complete!")
    time.sleep(0.8)
    
    os.system('cls' if os.name == 'nt' else 'clear')
    for i in range(0,23):
        print("\r")
    
    for i in bar(range(20), '                                                   Booting Operating System : ', 70):
        time.sleep(0.21)
        os.system('cls' if os.name == 'nt' else 'clear')
        for i in range(0,23):
            print("\r")
    
    print()
    print()
    print("                                                  ! Setup Complete !")
    print()
    winsound.PlaySound("Dependencies/Sound/OldWindows(Start).wav", winsound.SND_FILENAME)
    os.system('cls' if os.name == 'nt' else 'clear')

    winsound.PlaySound("Dependencies/Sound/idle.wav", winsound.SND_LOOP|winsound.SND_ASYNC)
    
    print()
    print("Coal OS - ver 1.4.6")
    print("Developed by 'Downtime' and 'Dr. Mass'")
    print("----------------------------------------------")
    print()
    uInput()
    
os.system('cls' if os.name == 'nt' else 'clear')
boot()
