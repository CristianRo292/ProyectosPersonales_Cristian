import os
from flask import Flask, render_template,redirect,request, session,flash
from helpers import apology, creacionTablas
from cs50 import SQL

app = Flask(__name__)
app.config["TEMPLATES_AUTO_RELOAD"] = True
app.secret_key = "con_esta_llave_nunca_se_cruzaran"
consultas = []
total = 0.0


# Inicializar base de datos una sola vez al iniciar la app
def init_db():
    if not os.path.exists("inventario.db"):
        open("inventario.db", "w").close()
    db = SQL("sqlite:///inventario.db")
    creacionTablas(db)
    return db

def datosSesion():
    if "consultas" not in session:
        session["consultas"] = []
    if "total" not in session:
        session["total"] = 0.0
    return 0

# Crear la conexión a la base de datos
db = init_db()

@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    datosSesion()
    return response

@app.route("/")
def index():
    try:
        datos = db.execute('''
            SELECT categoria, 
                SUM(precio_unitario * existencias) AS valor,
                SUM(existencias) AS existencias 
            FROM productos 
            GROUP BY categoria
        ''')
        return render_template("index.html", existencias=datos)
    except Exception as e:
        print("Error:", e)
        return apology("No se pudo cargar el inventario")

@app.route("/vender",methods=["GET", "POST"])
def vender():
    try:
        productosD = db.execute(
            "SELECT id,nombre FROM productos WHERE existencias >  0"
        )
        if request.method == "POST":
            for con in session["consultas"]:
                db.execute("UPDATE productos SET existencias = existencias - ? WHERE nombre = ?",con["unidades"], con["nombre"])
                db.execute("INSERT INTO ventas (id_producto,cantidad) VALUES (?,?)",con["id_producto"],con["unidades"])

            session["consultas"]= [] 
            session["total"] = 0.0
            flash("¡Venta realizada con éxito!", "success")
            return redirect("/")
        
        return render_template("vender.html", productos = productosD, ventas = session["consultas"], total = session["total"])
    except Exception as e:
        print("Error en /vender:",e)
        return apology("Ocurrio un error en Ventas")

@app.route("/comprar", methods=["GET", "POST"])
def comprar():
    try:
        productos = db.execute("SELECT nombre,id FROM productos ")

        if request.method == "POST":
            print("obtubimos las variables")
            # Obtener datos del formulario
            id_prod = request.form.get("nombreP")
            unidades = request.form.get("unidades")
            costo = request.form.get("costo")

            # Validaciones
            if not id_prod:
                return apology("Todos los campos son obligatorios")

            if not unidades.isdigit() or int(unidades) <= 0:
                return apology("Cantidad debe ser un entero positivo")
            
            if (not costo.isdigit()) or (int(costo) <= 0):
                return apology("Dato de Costo Incorrecto")

            unidades_int = int(unidades)
            costo_int = int(costo)
            
            producto = db.execute(
                "SELECT nombre FROM productos WHERE id = ?",
                id_prod
            )

            if producto:
                # Producto existe, entonces se actualizan las existencias
                db.execute(
                    "UPDATE productos SET existencias = existencias + ? WHERE id = ?",
                    unidades_int, id_prod
                )
                db.execute("INSERT INTO compras (id_producto,cantidad, costo_unitario) VALUES (?,?,?)",id_prod,unidades_int,costo_int)


            else:
                return apology("El Prooducto no esta dado de alta")

            flash("¡Compra realizada con éxito!", "success")
            return redirect("/")

        else:
            # GET: mostrar formulario con listas de proveedores y productos
            return render_template("comprar.html", productos=productos)

    except Exception as e:
        print("Error en /comprar:", e)
        return apology("Error al procesar la compra")
    

@app.route("/buscar", methods=["GET", "POST"])
def buscar():
    try:
        similitudes = {}
        
        if request.method == "POST":
            palabra = request.form.get("busqueda")
            if not palabra:
                return apology("Campo bacio")
              
            similitudes = db.execute(
                "SELECT * FROM productos WHERE nombre LIKE ?",
                f"%{palabra}%"
            )
        return render_template("buscar.html", similitudes = similitudes)
    except Exception as e:
        print("Error en /buscar:", e)
        return apology("Busqueda no esta disponible")

@app.route("/inventario")
def inventario():
    try:
        return render_template("inventario.html", similitudes = db.execute("SELECT * FROM productos") )
    except:
        return apology("El inventario a fallado")
    

@app.route("/nuevo_producto", methods=["GET", "POST"])
def nuevoProducto():
    try:
        if request.method == "POST":
            # se hace consulta para agregar producto
            nombre = request.form.get("nombre")
            categoria = request.form.get("categoria")
            descripcion = request.form.get("descripcion")
            precio_unitario = request.form.get("precio_unitario")
            #validaciones
            if not nombre:
                return apology("Nombre de producto no valido")
            
            if not categoria:
                return apology("Categoria de producto no valido")
            
            if not descripcion:
                return apology("Descripcion de producto no valido")
            if (not precio_unitario) or (not precio_unitario.isdigit()) or (int(precio_unitario )<= 0):
                return apology("Precio de producto no valido")
            
            similitudes = db.execute("SELECT id FROM productos WHERE nombre = ?",nombre)
            if similitudes:
                return apology("Producto repetido")
            
            db.execute("INSERT INTO productos (nombre,categoria, descripcion,precio_unitario) VALUES (?,?,?,?)",nombre,categoria,descripcion,int(precio_unitario))
            flash("¡Se ha registrado un nuevo Producto!", "success")
            return redirect("/")

        return render_template("nuevo_producto.html")
    except Exception as e:
        print("Error en / nuevo_producto:", e)
        return apology("Error al registrar producto")

@app.route("/procesar_venta", methods=["GET", "POST"])
def procesar_venta():
    try:

        # global total, consultas 
        id_P = request.form.get("nombreP")
        if (not id_P) or (not id_P.isdigit()):
            return apology("Formato de Poducto no valido")
        
        con_nombreP = db.execute("SELECT nombre FROM productos WHERE id=?",id_P)
        nombreP = con_nombreP[0]["nombre"]

        if not nombreP:
            return apology("No se encontro el producto")
        
        descripcion = request.form.get("descripcion")
        if not descripcion:
            descripcion = ""
            
        unidades = request.form.get("unidades")
        if not (unidades.isdigit()):
            return apology("Unidades no validas")
        
        precioP = db.execute("SELECT precio_unitario FROM productos WHERE nombre = ?",nombreP)

        session["total"] += float(precioP[0]["precio_unitario"]) * float(unidades)

        comprasRegitradas = {
            "nombre": nombreP,
            "id_producto": id_P,
            "unidades": int(unidades),
            "precio":float(precioP[0]["precio_unitario"]),
            "total":float(precioP[0]["precio_unitario"]) * float(unidades)
        }
        session["consultas"].append(comprasRegitradas)

        return redirect("/vender")
    except Exception as e:
        print("Error en / procesarCompra:", e)
        return apology("Error al procesar venta") 
    