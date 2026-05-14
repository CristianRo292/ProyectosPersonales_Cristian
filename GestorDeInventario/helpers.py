
from flask import  render_template
from cs50 import SQL
 


def apology(message, code=400):
    """Render message as an apology to user."""

    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [
            ("-", "--"),
            (" ", "-"),
            ("_", "__"),
            ("?", "~q"),
            ("%", "~p"),
            ("#", "~h"),
            ("/", "~s"),
            ('"', "''"),
        ]:
            s = s.replace(old, new)
        return s

    return render_template("apology.html", top=code, bottom=escape(message)), code

def creacionTablas(db):
    # Proveedores
    db.execute("""
        CREATE TABLE IF NOT EXISTS proveedores (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            empresa TEXT NOT NULL,
            nombre_contacto TEXT,
            correo TEXT
        )
    """)

    # Productos
    db.execute("""
        CREATE TABLE IF NOT EXISTS productos (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            nombre TEXT NOT NULL,
            descripcion TEXT NOT NULL,
            categoria TEXT NOT NULL,
            precio_unitario NUMERIC NOT NULL,
            existencias INTEGER NOT NULL DEFAULT 0 
        )
    """)
    db.execute("""
        CREATE TABLE IF NOT EXISTS compras (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            id_producto INTEGER NOT NULL,
            id_proveedor INTEGER,
            costo_unitario NUMERIC NOT NULL,
            cantidad INTEGER NOT NULL,
            fecha DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(id_producto) REFERENCES productos(id) ON DELETE CASCADE,
            FOREIGN KEY(id_proveedor) REFERENCES proveedores(id) ON DELETE RESTRICT
        )
    """)

    db.execute("""
        CREATE TABLE IF NOT EXISTS ventas (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            id_producto INTEGER NOT NULL,
            cantidad INTEGER NOT NULL CHECK (cantidad > 0),
            fecha DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(id_producto) REFERENCES productos(id)
        )
    """)

    # Índices
    db.execute("CREATE INDEX IF NOT EXISTS idx_proveedores_empresa ON proveedores(empresa)")
    db.execute("CREATE INDEX IF NOT EXISTS idx_proveedores_correo ON proveedores(correo)")
    db.execute("CREATE INDEX IF NOT EXISTS idx_productos_categoria ON productos(categoria)")
    db.execute("CREATE INDEX IF NOT EXISTS idx_productos_nombre ON productos(nombre)")
    db.execute("CREATE INDEX IF NOT EXISTS idx_productos_existencias ON productos(existencias)")
    db.execute("CREATE INDEX IF NOT EXISTS idx_compras_producto ON compras(id_producto)")
    db.execute("CREATE INDEX IF NOT EXISTS idx_compras_proveedor ON compras(id_proveedor)")
    db.execute("CREATE INDEX IF NOT EXISTS idx_compras_fecha ON compras(fecha)")
    db.execute("CREATE INDEX IF NOT EXISTS idx_ventas_producto ON ventas(id_producto)")
    db.execute("CREATE INDEX IF NOT EXISTS idx_ventas_fecha ON ventas(fecha)")

