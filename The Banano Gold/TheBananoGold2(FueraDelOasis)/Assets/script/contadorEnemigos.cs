using System.Collections;
using System.Collections.Generic;
using System.IO;
using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class contadorEnemigos : MonoBehaviour
{
    public static int contadorEnEliminados = 0;
    public TextMeshProUGUI puntuacion;
    public TextMeshProUGUI vidas;
    public static int contVidas = 3;
    public GameObject panel;
    public TextMeshProUGUI txt_enmigosEliminaso;
    public TextMeshProUGUI txt_labelPuntosMeta;
    public int PuntosDeMeta = 6;
    

    //public InputField input_nombre;
    public TMP_InputField input_nombre;
    // Start is called before the first frame update
    private void Awake()
    {
        StreamReader leer;
        if (File.Exists("Puntaje.csv"))
        {
            leer = File.OpenText("Puntaje.csv");
            string Datos = leer.ReadLine();
            string[] p = Datos.Split(",");
            contadorEnEliminados = int.Parse(p[1]);
            leer.Close();
            

        }
        

    }
    public void Guardar()
    {
        File.Delete("Puntaje.csv");
        StreamWriter escribir;
        escribir = File.AppendText("Puntaje.csv");
        string nom = input_nombre.text;
        escribir.WriteLine(nom + "," + contadorEnEliminados.ToString());
        //escribir.Write(nom + " , " + contadorEnEliminados.ToString());
        escribir.Close();
        panel.gameObject.SetActive(false);
        Application.Quit();

    }
    void Start()
    {
        panel.gameObject.SetActive(false);
        txt_labelPuntosMeta.text = "/" + PuntosDeMeta.ToString();
    }

    // Update is called once per frame
    void Update()
    {
        vidas.text = "X" + contadorEnemigos.contVidas.ToString();
        puntuacion.text = contadorEnemigos.contadorEnEliminados.ToString();
        if (contVidas <= 0)
        {
            Time.timeScale = 0;
            panel.gameObject.SetActive(true);
            txt_enmigosEliminaso.text =contadorEnemigos.contadorEnEliminados.ToString();
            return;
        }
        else if (contadorEnEliminados >= PuntosDeMeta)
        {
            // hacer animacion de la nave creciendo y volando

            //celebracion sigEsc = new celebracion();
            //sigEsc.InciarInterrupcion();
            Guardar();
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);

        }
            print(contadorEnEliminados);
        
        
    }
    
    public static void IncrementarEnemigo()
    {
        contadorEnEliminados ++;
    }
    public static void DescontarVidas()
    {
        contVidas--;
    }
    //public void celebracion()
    //{
    //    Vector3 posicion = new Vector3(-14.21646f, 14.6026f, -1);
    //    GameObject clon = Instantiate(celebracionFu, posicion, Quaternion.identity);
    //    Destroy(clon, 8.0f);
    //}
    
}
