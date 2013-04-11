using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;

namespace encrypt
{
    public class Crypto
    {
        //This is the salt used by RFC2898 object to generate the key for encryption
        private static byte[] _salt = Encoding.ASCII.GetBytes("8jtqop6ld0k");

        //method for encryption using a password
        public static string EncryptStringAES(string plainText, string password)
        {
            //temp string for the encrypted string
            string outStr = null;  
            //creates a Rijndeal encryptor object        
            RijndaelManaged aesAlg = null;              
            try
            {
                //creates a key by combining the password and salt using the RFC2898 onbject
                Rfc2898DeriveBytes key = new Rfc2898DeriveBytes(password, _salt);
                //new rijndeal ealgorythm for the encryptor
                aesAlg = new RijndaelManaged();
                //sets the key of the algorythim
                aesAlg.Key = key.GetBytes(aesAlg.KeySize / 8);
                //creates and ecryptor object using the RinhdealManager
                ICryptoTransform encryptor = aesAlg.CreateEncryptor(aesAlg.Key, aesAlg.IV);
                //the folowing code is using a memory stream called msEncrypt
                MemoryStream msEncrypt = new MemoryStream();
                //writes size and other info to the stream
                msEncrypt.Write(BitConverter.GetBytes(aesAlg.IV.Length), 0, sizeof(int));
                msEncrypt.Write(aesAlg.IV, 0, aesAlg.IV.Length);
                //creates and encrypted stream and pastes the result in msencrypt
                //and sets the ecryptor
                CryptoStream csEncrypt = new CryptoStream(msEncrypt, encryptor, CryptoStreamMode.Write);
                //creates another stream to read the text and feed to the encryptor stream
                StreamWriter swEncrypt = new StreamWriter(csEncrypt);
                //starts the text stream
                swEncrypt.Write(plainText);
                //sets the return string to the string value of msEncrypt
                outStr = Convert.ToBase64String(msEncrypt.ToArray());
                
            }
            finally
            {
                //clrears the encryptor to get rid of the reference
                if (aesAlg != null)
                    aesAlg.Clear();
            }
            //returns encrypted key as cypher
            return outStr;
        }

        //method for decrypting string
        public static string DecryptStringAES(string cipherText, string password)
        {
            //a cypher object
            RijndaelManaged aesAlg = null;
            //temp output string
            string plaintext = null;
            try
            {
                //creates a key using the password and salt
                Rfc2898DeriveBytes key = new Rfc2898DeriveBytes(password, _salt);
                //converts string into array ob bytes
                byte[] bytes = Convert.FromBase64String(cipherText);
                //starts a new memory stream with the text as content
                MemoryStream msDecrypt = new MemoryStream(bytes);
                //creates the new cypher object
                aesAlg = new RijndaelManaged();
                //sets the key
                aesAlg.Key = key.GetBytes(aesAlg.KeySize / 8);
                //sets the byte array to decrypt
                aesAlg.IV = ReadByteArray(msDecrypt);
                //new encryptor with the decryption agorythm
                ICryptoTransform decryptor = aesAlg.CreateDecryptor(aesAlg.Key, aesAlg.IV);
                //a new encryption stream with the encryptor in it aswell as the text
                CryptoStream csDecrypt = new CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Read);
                //main stream that cycles threw the text using the crypto stream
                StreamReader srDecrypt = new StreamReader(csDecrypt);
                //creating decrypted string from bytes.
                plaintext = srDecrypt.ReadToEnd();
            }
            finally
            {
                if (aesAlg != null)
                    aesAlg.Clear();
            }
            return plaintext;
        }


        //I did not write this method it can be found on the msdn website
        private static byte[] ReadByteArray(Stream s)
        {
            byte[] rawLength = new byte[sizeof(int)];
            if (s.Read(rawLength, 0, rawLength.Length) != rawLength.Length)
            {
                throw new SystemException("Stream did not contain properly formatted byte array");
            }
            byte[] buffer = new byte[BitConverter.ToInt32(rawLength, 0)];
            if (s.Read(buffer, 0, buffer.Length) != buffer.Length)
            {
                throw new SystemException("Did not read byte array properly");
            }
            return buffer;
        }
    }
}
