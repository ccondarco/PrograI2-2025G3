#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

vector<string> meses = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
vector<string> categoriasProductoVector = {"Celular", "Tablet", "Television", "Licuadora", "Microondas"};

struct Fecha
{
    int mes;
    int dia;
    int anio;
};

struct DatosCliente
{
    char CI_Cliente[30];
    char nombre[30];
    char apellido[30];
    Fecha fechaNacimiento;
    int telefono;
    char correo[30];
};

struct DatosEmpleado
{
    char CI_Empleado[10];
    char nombre[30];
    char apellido[30];
    int rango;
};

struct DatosProducto
{
    int codigo;
    char modelo[30];
    int categoria;
    float precioVenta;
    int stock;
    bool eliminado;
};

struct ventaProducto
{
    int numeroFactura;
    int codigoProducto;
    char CI_Cliente[10];
    char CI_Empleado[10];
    Fecha fechaDeVenta;
    int cantidad;
    float precioUnitario;
};

struct Reparaciones
{
    char CI_cliente[10];
    int codigoProducto;
    int numeroFactura;
    char descripcion[100];
    float precioReparacion;
    Fecha fechaReparacion;
};

struct datosFactura
{
    int numeroFactura;
    char CI_cliente[10];
    Fecha fecha_emision_factura;
    int metodo_pago;
};

void categoriasProductoMenu() { // consultar ^w^ y realizar un vector acorde
    cout << "\tCATEGORÍAS\n";
    cout << "\t\t1.Celular\n";
    cout << "\t\t2. Tablet\n";
    cout << "\t\t3. Televisión\n";
    cout << "\t\t4. Licuadora\n";
    cout << "\t\t5. Microondas\n";
}

DatosProducto insertarDatosProducto() {
    DatosProducto producto;

    cout << "Código: ";
    cin >> producto.codigo;
    cin.ignore();
    cout << "Modelo: ";
    cin.getline(producto.modelo, 30);
    cout << "Categoría: ";
    categoriasProductoMenu();
    cin >> producto.categoria;
    producto.categoria -= 1; // Rebajar en un número la categoría para que sea acorde al vector
    cout << "Precio de venta: ";
    cin >> producto.precioVenta;
    cout << "Stock: ";
    cin >> producto.stock;
    producto.eliminado = false; // Al agregar un producto, se lo coloca automáticamente como No eliminado
    
    return producto;
}

void agregarProducto (string nombreArchivo) {
    ofstream archivo;
    DatosProducto producto = insertarDatosProducto();
    archivo.open(nombreArchivo, ios::binary | ios::app); // se añaden datos al archivo
    if (archivo.good()) {
        archivo.write((char*)(&producto), sizeof(DatosProducto));
        cout << "Producto añadido con exito\n";
    } else {
        cout << "Error al añadir producto\n";
        return;
    }
    archivo.close();
}

void mostrarListaProductosPantalla(string nombreArhcivo) {
    ifstream archivo;
    DatosProducto producto;
    system("cls");

    archivo.open(nombreArhcivo, ios::binary);
    if (archivo.good()) {
        cout << "============ LISTA DE PRODUCTOS DISPONIBLES ===================\n";
        while(archivo.read((char*)(&producto), sizeof(DatosProducto)) && producto.eliminado==false) {
            cout << "\tCódigo: " << producto.codigo << endl;
            cout << "\tModelo: " << producto.modelo << endl;
            cout << "\tCategoría: " << categoriasProductoVector[producto.categoria] << endl;
            cout << "\tPrecio de venta: " << producto.precioVenta << endl;
            cout << "\tStock: " << producto.stock << endl;
            cout << "--------------------------------------------------------\n";
        }
        archivo.close();
        system("pause");

    } else {
        cout << "Error al abrir el archivo de productos\n";
        system("pause");
        return;
    }
}

void ModificarProducto(string nombreArchivo) { // Es posible que se deba hacer más fucniones para modificar una parte específica y no así todos los datos
    fstream archivo; // leer y modificar
    DatosProducto producto;
    int codigoBuscado; // se buscará por código
    bool encontrado = false;
    char confirmacion; // se hará una pregunta de confirmación para modificar el producto

    cout << "Ingrese el código del producto a modificar: ";
    cin >> codigoBuscado;

    //Abrir el archivo
    archivo.open(nombreArchivo, ios::in | ios::out | ios::binary);
    if (archivo.good()) {
        // Buscar el producto
        while (archivo.read((char*)(&producto), sizeof(DatosProducto))) {
            if (codigoBuscado==producto.codigo && encontrado==false) {
                cout << "=== PRODUCTO A MODIFICAR ===\n";
                cout << "\tCódigo: " << producto.codigo << endl;
                cout << "\tModelo: " << producto.modelo << endl;
                cout << "\tCategoría: " << producto.categoria << endl;
                producto.categoria -= 1;
                cout << "\tPrecio de venta: " << producto.precioVenta << endl;
                cout << "\tStock: " << producto.stock << endl;
                encontrado = true;

                cout << "¿Desea modificar este producto? (s/n): ";
                cin >> confirmacion;
                if (confirmacion == 's') {
                    archivo.seekp(-sizeof(DatosProducto), ios::cur);
                    cin.ignore();
                    cout << "NUEVOS DATOS DEL PRODUCTO\n";
                    cout << "Modelo: ";
                    cin.getline(producto.modelo, 30);
                    cout << "Categoría: ";
                    categoriasProductoMenu();
                    cin >> producto.categoria;
                    cout << "Precio de venta: ";
                    cin >> producto.precioVenta;
                    cout << "Stock: ";
                    cin >> producto.stock;
                    archivo.write((char*)(&producto), sizeof(DatosProducto));
                } else {
                    archivo.close();
                    return;
                }
            }
        }
        if (encontrado == false) {
            cout << "No se pudo encontrar el producto.\n";
        }
    } else {
        cout << "Error al eliminar producto\n";
    }
    archivo.close();
}

void eliminarProducto(string nombreArchivo) {
    fstream archivo; // leer y modificar
    DatosProducto producto;
    int codigoBuscado; // se buscará por código
    bool encontrado = false;
    char confirmacion; // se hará una pregunta de confirmación para eliminar el contacto

    cout << "Ingrese el código del producto a eliminar: ";
    cin >> codigoBuscado;

    //Abrir el archivo
    archivo.open(nombreArchivo, ios::in | ios::out | ios::binary);
    if (archivo.good()) {
        // Buscar el producto
        while (archivo.read((char*)(&producto), sizeof(DatosProducto))) {
            if (codigoBuscado==producto.codigo && encontrado==false) {
                cout << "=== PRODUCTO A ELIMINAR ===\n";
                cout << "\tCódigo: " << producto.codigo << endl;
                cout << "\tModelo: " << producto.modelo << endl;
                cout << "\tCategoría: " << producto.categoria << endl;
                cout << "\tPrecio de venta: " << producto.precioVenta << endl;
                cout << "\tStock: " << producto.stock << endl;
                encontrado = true;

                cout << "¿Desea eliminar este producto? (s/n): ";
                cin >> confirmacion;
                if (confirmacion == 's') {
                    archivo.seekp(-sizeof(DatosProducto), ios::cur);
                    archivo.write((char*)(&producto), sizeof(DatosProducto));
                } else {
                    archivo.close();
                    return;
                }
            }
        }
        if (encontrado == false) {
            cout << "No se pudo encontrar el producto.\n";
        }
    } else {
        cout << "Error al eliminar producto\n";
    }
    archivo.close();
}

void menuABM_Productos(string nombreArchivoBin) {
    int opcion;
    do {
        system("cls");
        cout << "== MENÚ PRODUCTOS ==\n";
        cout << "\t1. Agregar producto\n";
        cout << "\t2. Modificar producto\n";
        cout << "\t3. Eliminar producto\n";
        cout << "\t4. Mostrar lista de productos disponibles\n";
        // cout << "\t4. Reporte Mensual de Reapraciones\n";
        // cout << "\t5. Mostrar producto más veces reparado\n";
        cout << "\t0. Volver\n";
        cout << "--> ";
        cin >> opcion;
        if (opcion==1) {
            agregarProducto(nombreArchivoBin);
        } else if (opcion==2) {
            ModificarProducto(nombreArchivoBin);
        } else if (opcion==3) {
            eliminarProducto(nombreArchivoBin);
        } else if(opcion==4) {
            mostrarListaProductosPantalla(nombreArchivoBin);
        } else if (opcion==5) {

        } else if (opcion==0) {
            cout << "Volviendo al menú principal...\n";
        }
    } while (opcion!=0);
}

// -------------------------------------------------------------------------------------------------------------------

Reparaciones DatosReparacion() {
    Reparaciones reparacion;
    Fecha datosFechaTemporal;

    cout << "CI del cliente: ";
    cin >> reparacion.CI_cliente;
    cout << "Código del producto: ";
    cin >> reparacion.codigoProducto;
    cin.ignore();
    cout << "Descripción de los daños(100 caracteres): ";
    cin.getline(reparacion.descripcion, 100);
    cout << "Fecha: \n";
    cout << "\tDía: ";
    cin >> datosFechaTemporal.dia;
    cout << "\tMes: ";
    cin >> datosFechaTemporal.mes;
    cout << "\tAño: ";
    cin >> datosFechaTemporal.anio;
    reparacion.fechaReparacion = datosFechaTemporal;
    cout << "Precio de venta: "; // 1 anio de garantia
    cin >> reparacion.codigoProducto;
   
    return reparacion;
}

void agregarDatosReparacion(string nombreArchivo) {
    ofstream archivo;
    Reparaciones repararacion;

    archivo.open(nombreArchivo, ios::binary | ios::app);
    if (archivo.good()) {
        repararacion = DatosReparacion();
        archivo.write((char*)(&repararacion), sizeof(Reparaciones));
        archivo.close();
        cout << "Datos de la reparación agregados con éxito.";
        system("pause");
    } else {
        cout << "Error al agregar los datos de la reparación";
        system("pause");
        return;
    }
}

void encontrarReparacion(string nombreArchivo) {
    fstream archivo;
    Reparaciones reparacion;
    bool encontrado = false;
    char ciBuscado[10];
    int codigoProductoBuscado;
    int numeroFacturaBuscada;

    archivo.open(nombreArchivo, ios::in | ios::out | ios::binary);
    if (archivo.good()) {
        cout << "== DATOS PARA ENCONTRAR LA REPARACIÓN ==\n";
        cout << "\tCI: ";
        cin.ignore();
        cin.getline(ciBuscado,10);
        cout << "\tCódigo del Producto: ";
        cin >> codigoProductoBuscado;
        cout << "\tNúmero de la Factura: ";
        cin >> numeroFacturaBuscada;
        while(archivo.read((char*)(&reparacion), sizeof(Reparaciones)) && encontrado==false) {
            if ((strcmp(ciBuscado,reparacion.CI_cliente)==0) && codigoProductoBuscado==reparacion.codigoProducto && 
            numeroFacturaBuscada==reparacion.numeroFactura) {
                system("cls");
                cout << "REGISTRO DE REPARACIÓN ENCONTRADO\n";
                cout << "\tCliente: " << reparacion.CI_cliente << endl;
                cout << "\tCodigo del Producto: " << reparacion.codigoProducto << endl;
                cout << "\tNúmero de Factura: " << reparacion.numeroFactura << endl;
                cout << "\tFecha: " << reparacion.fechaReparacion.dia << "/" << reparacion.fechaReparacion.mes << "/" << reparacion.fechaReparacion.anio << endl;
                cout << "\tPrecio de la Reparación: " << reparacion.precioReparacion << endl;
                cout << "\tDescripción: " << reparacion.descripcion << endl;
                cout << "-----------------------------------------------------\n";
            }
        }
        if (encontrado==false) {
            cout << "Reparación no encontrada";
        }
    } else {
        cout << "Error al encontrar registro de reparación los datos de la reparación";
        system("pause");
        return;
    }
}

void modificarDatosReparacion(string nombreArchivo) {
    fstream archivo;
    Reparaciones reparacion;
    bool encontrado = false;
    char ciBuscado[10];
    int codigoProductoBuscado;
    int numeroFacturaBuscada;

    archivo.open(nombreArchivo, ios::in | ios::out | ios::binary);
    if (archivo.good()) {
        cout << "== DATOS PARA ENCONTRAR LA REPARACIÓN ==\n";
        cout << "\tCI: ";
        cin.ignore();
        cin.getline(ciBuscado,10);
        cout << "\tCódigo del Producto: ";
        cin >> codigoProductoBuscado;
        cout << "\tNúmero de la Factura: ";
        cin >> numeroFacturaBuscada;
        while(archivo.read((char*)(&reparacion), sizeof(Reparaciones)) && encontrado==false) {
            if ((strcmp(ciBuscado,reparacion.CI_cliente)==0) && codigoProductoBuscado==reparacion.codigoProducto && 
            numeroFacturaBuscada==reparacion.numeroFactura) {
                system("cls");
                cout << "REGISTRO DE REPARACIÓN ENCONTRADO\n";
                cout << "\tCliente: " << reparacion.CI_cliente << endl;
                cout << "\tCodigo del Producto: " << reparacion.codigoProducto << endl;
                cout << "\tNúmero de Factura: " << reparacion.numeroFactura << endl;
                cout << "\tFecha: " << reparacion.fechaReparacion.dia << "/" << reparacion.fechaReparacion.mes << "/" << reparacion.fechaReparacion.anio << endl;
                cout << "\tPrecio de la Reparación: " << reparacion.precioReparacion << endl;
                cout << "\tDescripcion: " << reparacion.descripcion << endl;
                cout << "-----------------------------------------------------\n";
                cout << "DATOS NUEVOS\n";
                archivo.seekp(-sizeof(Reparaciones), ios::cur);
                cout << "Descripción: ";
                cin.getline(reparacion.descripcion,100);
                archivo.write((char*)(&reparacion), sizeof(Reparaciones));
                archivo.close();
                cout << "Registro de Reparación modificado con éxito\n";
            }
        }
        if (encontrado==false) {
            cout << "Reparación no encontrada";
        }
    } else {
        cout << "Error al modificar los datos de la reparación";
        system("pause");
        return;
    }
}

//--------Reportes Reparaciones--------------
void imprimirReporteMensualReparaciones (string nombreArchivoBin, string nombreArchivoTxt) {
    ofstream archivoTexto; //escribir
    ifstream archivoBin; //leer
    Reparaciones reparacion;
    int mesBuscado;
    int anioBuscado;

    archivoBin.open(nombreArchivoBin, ios::binary);
    archivoTexto.open(nombreArchivoTxt, ios::app);
    if (archivoBin.good() && archivoTexto.good()) {
        cout << "REPORTE MENSUAL DE REPARACIONES\n";
        cout << "Mes del reporte: ";
        cin >> mesBuscado;
        cout << "Año del reporte: ";
        cin >> anioBuscado;
        archivoTexto << "===== REPARACIONES DEL MES DE " << meses[mesBuscado-1] << " DE " << anioBuscado << " =====\n";    
        while(archivoBin.read((char*)(&reparacion), sizeof(Reparaciones))) {
            if (reparacion.fechaReparacion.mes==mesBuscado && reparacion.fechaReparacion.anio==anioBuscado) {
                archivoTexto << "--------------------------------------------------\n";
                archivoTexto << "Fecha: " << reparacion.fechaReparacion.dia << "/" << reparacion.fechaReparacion.mes << "/" << reparacion.fechaReparacion.anio << endl;
                archivoTexto << "DATOS DEL CLIENTE\n";
                archivoTexto << "CI: " << reparacion.CI_cliente; // buscar los datos del cliente en otro archivo
                archivoTexto << "DATOS DEL PRODUCTO\n";
                archivoTexto << reparacion.codigoProducto; // buscar con el codigo del producto;

            }
        }
    } else {
        cout << "Error al abrir el archivo de reparaciones\n";
    }
}

void menuABM_Reparacion(string nombreArchivoBin) {
    int opcion;
    do {
        cout << "== MENÚ REPARACIONES ==\n";
        cout << "\t1. Agregar registro de reparación\n";
        cout << "\t2. Encontrar registro de reparación\n";
        cout << "\t3. Modificar descripción de reaparación\n";
        cout << "\t4. Reporte Mensual de Reapraciones\n";
        cout << "\t5. Mostrar producto más veces reparado\n";
        cout << "\t0. Volver\n";
        cout << "--> ";
        cin >> opcion;
        if (opcion==1) {
            agregarDatosReparacion(nombreArchivoBin);
        } else if (opcion==2) {
            encontrarReparacion(nombreArchivoBin);
        } else if (opcion==3) {
            modificarDatosReparacion(nombreArchivoBin);
        } else if(opcion==4) {

        } else if (opcion==5) {

        } else if (opcion==0) {
            cout << "Volviendo al menú principal...\n";
        }
    } while (opcion!=0);

}