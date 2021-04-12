import argparse
import socket
import re
import os

#This function gets server adress from nameserver
#server_name is string containing name of server
#nameserver is string containing name server address
#returns "None" in case of error OR server_address in case of success
def get_server_address(server_name, nameserver):
    #Creating message
    nameserver_msg = "WHEREIS " + server_name
    #Creating right format for address
    address = nameserver.split(":")
    address[1] = int(address[1])
    if address[1] > 65535:
        print("Error: Bad Port")
        return None
   
    s_udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) #Creating udp socket
    s_udp.settimeout(35.0); #Setting timeout
    
    #Waiting for server to respond
    try:
        s_udp.sendto(nameserver_msg.encode(), tuple(address))
        # Receives answer from nameserver in form "OK <IP>" in byte string and decodes it
        received = s_udp.recvfrom(1024)[0].decode()
    except socket.timeout:
        print("Error: Nameserver timed out")
        return None;

    # Receives answer from nameserver in form "OK <IP>" in byte string and decodes it
    #received = s_udp.recvfrom(1024)[0].decode()

    # Error check
    if re.search(r"OK [0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}:[0-9]{1,5}", received) == None:
        print("Error: Address of server wasn't found on nameserver")
        return None

    # Cuts address out of whole server answer
    fsp_server_addr = re.sub(r'OK ', '', received)
    return fsp_server_addr

#Creates fsp_get reguest depending on filepath and server_name
#filepath -> string containing path to file on server with server_name
#server_name -> string containing name of server
#returns -> string containing request
def create_fsp_get_request(filepath, server_name):
    request = "GET " + filepath + " FSP/1.0\r\n" + \
        "Hostname: " + server_name + "\r\n" + "Agent: xpopel24\r\n\r\n"
    return request

#Extracts server_name, file_path and filename from SURL
#surl -> string containing surl
#returns -> dictionary containing server_name, filepath,filename
def decompose_surl(surl):
    tmp = re.sub(r'fsp://', '', surl)
    server_name = re.search(r'[^/]*', tmp).group()
    filepath = re.search(r'/.*', tmp).group()[1:]
    filename = filepath.split('/')[-1]
    return {"server_name": server_name, "filepath": filepath, "filename": filename}


#Checks if surl and nameserver, arguments passed to program on start, are valid
#surl-> string containing surl
#nameserver-> string contaning nameserver ip_address
#returns-> True if arguments are valid, False if arguments are invalid
def check_arg_validity(surl, nameserver):
    if re.search(r"^fsp://[.\-\w]+/.+$", surl) and re.search(r"^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}:[0-9]{1,5}$", nameserver):
        for num in nameserver.split(":")[0].split("."):
            if int(num) > 255:
                return False
        return True
    return False

#Main function, that is launched in script, this function executes or functions mentioned before
#and downloads file/files depending on filepath
#surl -> string containing surl
#nameserver -> string containing ip and port of nameserver
#returns 0 if everything went ok, 1 if error ocurred
def send_request(surl, nameserver):
    #checks if args are valid
    if check_arg_validity(surl, nameserver) == False:
        print("Error: Incorrect arguments")
        return 1

    #Decomposes url
    file_info = decompose_surl(surl)
    #gets server address
    server_address = get_server_address(file_info["server_name"], nameserver)

    #err check
    if server_address == None:
        return 1
    

    #gets filepath
    filepaths= [file_info["filepath"]]

    #if filename is "*" downloads index first and then all other files
    if file_info["filepath"] == "*":
        file_info["filename"] = "index"
        file_info["filepath"] = "index"
        filepaths = download_file(file_info,server_address,True)

        if filepaths == False:
            return 1
        
        #Extends filepaths variable by filepaths of all files on server
        filepaths = filepaths.split("\r\n")[:-1]

    #Looping through filepaths and downloading files
    for filepath in filepaths:
        file_info["filepath"] = filepath
        file_info["filename"] = file_info["filepath"].split('/')[-1]
        if download_file(file_info,server_address,False) == False:
            return 1
    return 0
    
#Splits filename to filename and it's extension
#for renaming already ocurring files
#filename -> string containing filename
#returns tuple containing filename on index 0 and extension on index 2
def split_filename_extension(filename):
   splitted_filename = filename.split(".")
   return ".".join(splitted_filename[:-1]),splitted_filename[-1] 

#Downloads file from server using TCP connection
#file_info -> dictionary containing file_info
#server_address -> string containing address of server
#is_index -> boolean value telling us whether we download index for get_all function (if true, it won't download to file)
def download_file(file_info: dict, server_address:str, is_index: bool):
    
    #Creates tcp connection
    s_tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s_tcp.settimeout(50.0)
    address = server_address.split(":")
    address[1] = int(address[1])
    s_tcp.connect(tuple(address))

    #creating request
    request = create_fsp_get_request(
    file_info["filepath"], file_info["server_name"])

    #sending request
    s_tcp.sendall(request.encode())
    
    #Recieving and decoding first line
    first_line = s_tcp.recv(17).decode().rstrip()

    #If we get err msg, False is returned
    if first_line != "FSP/1.0 Success":
        print("Error: File wasn't found!")
        s_tcp.close()
        return False

    #Getting length line from server response
    length_line = s_tcp.recv(12).decode()
    while(length_line.find("\r\n\r\n") < 0):
        length_line += s_tcp.recv(1).decode()
    

    #Creating new filename in form  "filename(number).extension" in case file with same name exists in folder already
    splitted_filename = split_filename_extension(file_info["filename"])
    counter = 1
    while os.path.exists(file_info["filename"]):
        file_info["filename"] = splitted_filename[0] + "(" + str(counter) + ")." + splitted_filename[1]
        counter += 1 

    #Downloading file contains
    if is_index == False:
        out_file = open(file_info["filename"], "wb")
    content = s_tcp.recv(4096)
    whole_content = content
    while content != b'':
        if is_index == False:
            out_file.write(content)
        content = s_tcp.recv(4096)

        if is_index:
            whole_content += content
    s_tcp.close();

    if is_index == False:
        out_file.close()
    else:
        return whole_content.decode()


arg_parser = argparse.ArgumentParser(
    description="Downloads file from given server", add_help=False)
required_args = arg_parser.add_argument_group("required arguments")
required_args.add_argument('-n', required=True)
required_args.add_argument('-f', required=True)
args = arg_parser.parse_args()
exit(send_request(args.f,args.n))
