using System.Collections;
using System.Collections.Generic;
using Unity.Mathematics;
using UnityEngine;

public class moverPersonaje : MonoBehaviour
{
    Rigidbody2D cuerpo_nave;
    float cordX, cordY;
    public int velocidad = 10, velocidadRotacion = 10;
    private Camera camara;
    public float margen = 0.5f;

    // Start is called before the first frame update
    void Start()
    {
        cuerpo_nave = GetComponent<Rigidbody2D>(); // optengo compoentes del rigi body
        camara = Camera.main;
        
    }

    // Update is called once per frame
    void Update()
    {
        float altura = camara.orthographicSize; // | se optiene la medida de la distancia entre el centro y el borde superior de la camara
        float ancho = altura * camara.aspect; // -- se multiplica el valor de su altura (ejem 5) por su relacion de aspecto (ejem 16:9 == 1.77) para obtener el valor de su ancho (5 * 1.777 = 8.88)
        Vector3 posicion = transform.position;

        posicion.x = Mathf.Clamp(posicion.x, -(ancho) + margen, ancho - margen); // >:| obligamos a que el personaje se mantega dentro de un rago de valores,
                                                                                 // lo aclamos (posicionActual, posicionMinima, posicionMaxima)
        posicion.y = Mathf.Clamp(posicion.y, -(altura) + margen, altura - margen);

        transform.position = posicion; // :) Cargamos las restricciones a la posicion del personaje

        cordX = Input.GetAxis("Horizontal");
        cordY = Input.GetAxis("Vertical");
        //print("coordenaX: " + cordX + "Coordeanda Y: " + cordY);
        //print("coordenaX: " +  cordX + "Coordeanda Y: " + cordY);

        cuerpo_nave.velocity = transform.up * velocidad * cordY; // mandamos a mover la nave respecto al eje y (up)
        cuerpo_nave.angularVelocity = -(cordX) * velocidadRotacion * 100;

        //cuerpo_nave.velocity = transform.right * velocidad * cordX; // mandamos a mover la nave respecto al eje X (rigt)
        //cuerpo_nave.velocity = ((transform.right * cordX) + (transform.up * cordY)) * velocidad;
        
    }
    
}
