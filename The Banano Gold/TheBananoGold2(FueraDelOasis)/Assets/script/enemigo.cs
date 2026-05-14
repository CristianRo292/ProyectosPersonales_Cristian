using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class enemigo : MonoBehaviour
{
    public float velocidad = 10f;
    public float velocidadPersecucion = 8f;
    Vector2 posicion;
    public float distancia = 200f;
    public bool seguir_Personaje = false;
    public Transform objetivo;
    float timer;

    // Start is called before the first frame update
    void Start()
    {
        posicion = transform.position;
        objetivo = GameObject.FindGameObjectWithTag("Player").transform;
    }

    // Update is called once per frame
    void Update()
    {
       
        if (seguir_Personaje)
        {
            seguirPersonaje();
            return;
        }
        transform.Translate(Vector3.down * velocidad *  Time.deltaTime);
        float inicio = Vector2.Distance(posicion, transform.position);
        inicio = Mathf.Abs(inicio);
        if (inicio > distancia)
        {
            Destroy(gameObject);
        }


    }
    public void seguirPersonaje()
    {
        if (objetivo == null) return;

        Vector3 direccion = (objetivo.position - transform.position).normalized;
        transform.position += direccion * velocidadPersecucion * Time.deltaTime;
        transform.up = - (direccion);
        timer += Time.deltaTime;
    }
    public void destruirPorDistancia()
    {

    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.CompareTag("bala"))
        {
            contadorEnemigos.IncrementarEnemigo();
            Destroy(collision.gameObject);
            
        }
        else if (collision.CompareTag("Player"))
        {
            contadorEnemigos.DescontarVidas();
        }
        else
        {
            return;
        }
            Destroy(gameObject);
    }
}
