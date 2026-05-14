using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using Unity.VisualScripting.Antlr3.Runtime;
using UnityEngine;

public class controladorArchivo
{
    public StreamWriter archivo_escribir;
    public StreamReader archivo_leer;
    public string ExtrarDatos(string nombre = "pajaro.txt")
    {
        try
        {
            string datos = "  ,  ";
            if (File.Exists(nombre))
            {
                archivo_leer = File.OpenText("pajaro.txt");
                //print("Leemos el archivo");
                datos = archivo_leer.ReadLine();
                archivo_leer.Close();
               
            }
            return datos;
        }
        catch (Exception ex)
        {
            Debug.LogError(ex.Message);
            return null;
        }
        
    }
    public bool ModificarAarchivo(string nombre = "pajaro.txt", string parametro = null)
    {
        try
        {
            if (File.Exists(nombre))
            {
                File.Delete(nombre);
            }
            archivo_escribir = File.AppendText(nombre);
            archivo_escribir.WriteLine(parametro);
            archivo_escribir.Close();
            return true;
        }
        catch (Exception ex)
        {
            Debug.LogError("Error: " + ex.Message);
            return false;
        }
    }
    public bool VerificarExistencia(string nombre = "pajaro.txt")
    {
        return false;
    }
}
