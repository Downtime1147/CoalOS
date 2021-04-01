# CoalOS


Welcome, and thank you for taking the time to check out CoalOS !


CoalOS is a simulator where you can spend your time using fictional hacking tools to break your way
into many different connections and/or buisneses (listed in the 'iwlist' command.)

If you are new to this sort of thing, I would suggest reading the rest of this document
and the 'help' list to learn the basics of this game.


( ! This does play sound so you can wear headphones if you want ! )

---------------------------------------------------------------------------------------------------------------------------------------

Part One : Getting started
---------------------------

- On start, make sure it is in full screen otherwise some graphics might not look right!

- Next, (if you haven't used this already) type 'help' to view a full list of commands that you can use.

- You can then start hacking stuff. There's not much you can do with it right now but that will change
  as I work on more updates!

  Here is how you hack stuff:

       - First, type 'iwlist' to get a list of connections you can hack.
       
         You will notice there are different statuses for each connection (Closed, Dead, and Open.):

              - Open means you can hack it using standard tools and are much easier (aireplay for password and aircon to connect)

              - Closed means you can still hack it but it might requier a little more effort to do so (And some different tools)

              - Dead means that the connection is non-functional and you can't do anything with it (it is innactive)

       - Now that you know what those mean, we can start hacking into our first wireless connection.

       - Open Connections:

            1. If you don't have it open already, type 'iwlist' to bring up a list of connections

            2. Pick a connection with a 'OPEN' status

            3. Type 'aireplay' to start the password cracker

            4. Enter the IP Address and ESSID, where prompted

            5. Sit back and watch the magic happen (Once its done, it should display a password)

            6. You can now type 'aircon', enter the IP Address and Password, then your done!

       - There you go! You have just bruteforced somebodies password and connected to their device (or Internet) !
         Have this ascii cookie, just for you!

   
        ________
       .        .
      |  O       |
      |     O    |   <-- There not raisns, I promise!
      |         O|
       .__O_____.

       - Closed Connections:

            A good thing to take note of, before we start, is that hacking closed connections involves a bit
            of trial and error so if you get it wrong the first time then it's not going to end the world.

            1. Once you have iwlist open, then pick a closed connection to hack

            2. Then, we are going to be making use of the 'nmap' command, so type that in, and then enter the
               IP Address of your chosen target (it should display a table with some ports and MAC Addresses)

            3. Next, your going to have to figure out if the port is vulnerable to FTP or SSH attacks.
               You do this just through trial and error (as explained above.) You do this by using one of the
               two tools that I added:

                 - 'FTpea' (by PeasInaPod417)
                 
                 and
 
                 - 'SScrack' (by UnfriendlyLocksmith123)

               All you do is just enter one of those commands above (type help oncce your in if you don't know what to do)
               Then, when prompted, just enter the requested information (IP Address, MAC Address, Port Number)

               It should have given you a password, which you can now type into that 'aircon' command (along with the IP address)

               If it dosen't and instead gives you a message like this,
                                             "The IP/MAC/Portnumber [input here] is invalid, is protected againts this attack, or already open..."
               Then you either:
               
                    = Typed one of the inputs wrong (You can copy paste text in if you prefer)
                    = That connection is protected from that attack (Just use another port cracking tool. Again, trial and error)
                    = The port is already open, which means you just dont have to do that at all (just use 'aireplay')


           4. After that, if you did it right, then your done! You have again managed to completely cripple somebodys
              digital security systems withought being caught and arrested, Nice work!

--------------------------
 End
--------------------------

Thanks, again for taking a look at this, and I hope you found this (even just mildly) entertaining!

Have fun hacking stuff!
