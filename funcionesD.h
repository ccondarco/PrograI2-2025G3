#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

vector<string> meses = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
vector<string> categoriasProductoVector = {"Celular", "Tablet", "Laptop", "Televisor", "Monitor", "Parlante", "Proyector", "Lavadora", "Refrigerador"};
vector<int> diasMeses = {0,31,28,31,30,31,30,31,31,30,31,30,31};
vector<int> diasMesesBisiesto = {0,31,28,31,30,31,30,31,31,30,31,30,31}; // se daj el primer componente como 0 para que vaya de acuerdo a los meses

struct Fecha
{
    int mes;
    int dia;
    int anio;
};

struct DatosCliente
{
    char CI_Cliente[10];
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
    char CI_Cliente[10];
    char CI_Empleado[10];
    Fecha fechaDeVenta;
    int codigoProducto;
    int cantidad;
    float precioUnitario;
    bool eliminada;
    bool facturada;
    int numeroFactura;
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
    bool anulada;
};

void categoriasProductoMenu() {
    cout << "\tCATEGORÍAS\n";
    cout << "\t\t1. Celular\n";
    cout << "\t\t2. Tablet\n";
    cout << "\t\t3. Laptop\n";
    cout << "\t\t4. Televisor\n";
    cout << "\t\t5. Monitor\n";
    cout << "\t\t6. Parlante\n";
    cout << "\t\t7. Proyector\n";
    cout << "\t\t8. Lavadora\n";
    cout << "\t\t9. Refrigerador\n";
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

void buscarProductoPorCodigo(string nombreArchivo) {
    ifstream archivo;
    DatosProducto producto;
    bool encontrado = false;
    int codigoBuscado;

    cout << "Ingrese el código del producto que busca: ";
    cin >> codigoBuscado;

    archivo.open(nombreArchivo, ios::binary);
    if (archivo.good()) {
        while (archivo.read((char*)(&producto), sizeof(DatosProducto)) && encontrado==false) {
            if (producto.codigo == codigoBuscado && producto.eliminado==false) {
                cout << "== PRODUCTO ENCONTRADO ==\n";
                cout << "Código: " << producto.codigo << endl;
                cout << "Modelo: " << producto.modelo << endl;
                cout << "Categoría: " << producto.categoria << endl;
                cout << "Precio: " << producto.precioVenta << endl;
                cout << "Stock: " << producto.stock << endl;
                cout << "=========================\n";
                encontrado = true;
            }
        }
        if (encontrado==false) {
            cout << "No se pudo encontrar un producto con ese código: ";
            system("pause");
            return;
        }
        system("pause");
        archivo.close();
    } else {
        cout << "Error al buscar el producto\n";
        system("pause");
        return;
    }
}

void productosBajoStock(string nombreArchivo) {
    ifstream archivo;
    DatosProducto producto;

    archivo.open(nombreArchivo, ios::binary);
    if (archivo.good()) {
        cout << "====== PRODUCTOS DE BAJO STOCK ======\n";
        while (archivo.read((char*)(&producto), sizeof(DatosProducto))) {
            if (producto.stock<5) {
                cout << "Código: " << producto.codigo << endl;
                cout << "Modelo: " << producto.modelo << endl;
                cout << "Categoría: " << producto.categoria << endl;
                cout << "Precio: " << producto.precioVenta << endl;
                cout << "Stock: " << producto.stock << endl;
                cout << "=========================\n";
            }
        }
        system("pause");
        archivo.close();
    } else {
        cout << "Error al buscar productos\n";
        system("pause");
        return;
    }
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
        cout << "\t5. Buscar producto por código\n";
        cout << "\t6. Reporte: productos con bajo stock (menos de 5 unidades)\n";
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
            buscarProductoPorCodigo(nombreArchivoBin);
        } else if (opcion==6) {
            productosBajoStock(nombreArchivoBin);
        }else if (opcion==0) {
            cout << "Volviendo al menú principal...\n";
        }
    } while (opcion!=0);
}

// -------------------------------------------------------------------------------------------------------------------

Fecha encontrarFechaFactura (string nombreArchivoFacturas, int numeroFacturaBuscado) {
    ifstream archivoFacturas;
    datosFactura factura;
    bool encontrado = false;
    Fecha fechaADevolver;

    archivoFacturas.open(nombreArchivoFacturas, ios::binary);
    if (archivoFacturas.good()) {
        while(archivoFacturas.read((char*)(&factura), sizeof(datosFactura)) && encontrado==false) {
            if (factura.numeroFactura==numeroFacturaBuscado && factura.anulada==false) {
                fechaADevolver = factura.fecha_emision_factura;
                encontrado = true;
            }
        }
        if (encontrado==false) {
            cout << "No se pudo encontrar la factura de la compra. No se puede ofrecer ningún descuento.\n";
            system("pause");
        }
        archivoFacturas.close();
    } else {
        cout << "Error al abrir el archivo de facturas\n";
        system("pause");
    }
    return fechaADevolver;
}

bool anioEsBisiesto(int anio) {
    if (anio%4==0) {
        if (anio%100==0) {
            if (anio%400==0) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

Reparaciones DatosReparacion(char ciClient[], int codigoProd, int numFactura, string nombreArchivoFacturas) {
    Reparaciones reparacion;
    Fecha datosFechaTemporal;
    Fecha fechaFactura;
    ifstream archivoFacturas;
    datosFactura factura;
    bool anioBisiesto;
    int diasDeDiferenciaEntreFechas;

    //cout << "CI del cliente: ";
    //cin >> reparacion.CI_cliente;
    strcpy(reparacion.CI_cliente,ciClient);
    //cout << "Código del producto: ";
    //cin >> reparacion.codigoProducto;
    reparacion.codigoProducto = codigoProd;
    reparacion.numeroFactura = numFactura;
    cout << "Descripción de los daños(99 caracteres): ";
    cin.getline(reparacion.descripcion, 100);
    cout << "Fecha: \n";
    cout << "\tDía: ";
    cin >> datosFechaTemporal.dia;
    cout << "\tMes: ";
    cin >> datosFechaTemporal.mes;
    cout << "\tAño: ";
    cin >> datosFechaTemporal.anio;
    reparacion.fechaReparacion = datosFechaTemporal;

    fechaFactura = encontrarFechaFactura(nombreArchivoFacturas, numFactura);
    // sumar años incompletos
    for (int i=fechaFactura.mes; i<13; i++) {
        if (anioEsBisiesto(fechaFactura.anio) && fechaFactura.mes<=2) {
            diasDeDiferenciaEntreFechas += diasMeses[i];
        } else {
            diasDeDiferenciaEntreFechas += diasMeses[i];
        }
    }
    // sumar años completos
    for (int i=fechaFactura.anio+1; i<reparacion.fechaReparacion.anio; i++) {
        if (anioEsBisiesto(i)) {
            diasDeDiferenciaEntreFechas += 366;
        } else {
            diasDeDiferenciaEntreFechas += 365;
        }
    }
    if (diasDeDiferenciaEntreFechas > 365) {
        cout << "La garantía ha expirado\nIntroduzca el precio de la reparación: ";
        cin >> reparacion.precioReparacion;
    } else {
        cout << "La garantía continúa vigente, la repración no tendrá costo\n";
        reparacion.precioReparacion = 0;
    }

    return reparacion;
}

void agregarDatosReparacion(string nombreArchivoReparacion, string nombreArchivoFacturas, string NombreArchivoCliente, string nombreArchivoProductos) {
    ofstream archivoReparacion;
    ifstream archivoClientes;
    ifstream archivoFacturas;
    ifstream archivoProductos;
    Reparaciones repararacion;
    DatosCliente cliente;
    datosFactura factura;
    DatosProducto producto;
    char CIClienteBuscado[10];
    bool encontradoCliente=false;
    int numFacturaBuscada;
    bool encontradoFactura=false;
    int codigoProductoBuscado;
    bool encontradoCodigo=false;

    cout << "DATOS DE REPARACIÓN\n";
    archivoClientes.open(NombreArchivoCliente, ios::binary);
    if (archivoClientes.good()) {
        cin.ignore();
        cout << "CI del Cliente: ";
        cin.getline(CIClienteBuscado,10);
        while (archivoClientes.read((char*)(&cliente), sizeof(DatosCliente)) && encontradoCliente==false) {
            if (strcmp(cliente.CI_Cliente,CIClienteBuscado)==0) {
                encontradoCliente == true;
            }
        }
        if (encontradoCliente==false) {
            cout << "No se ha encontrado a un cliente con ese CI\n";
            return;
        }
    } else {
        cout << "Error al abrir el archivo de clientes\n";
        return;
    }
    archivoFacturas.open(nombreArchivoFacturas, ios::binary);
    if (archivoFacturas.good()) {
        cout << "Número de Factura: ";
        cin >> numFacturaBuscada;
        while (archivoFacturas.read((char*)(&factura), sizeof(datosFactura)) && encontradoFactura==false) {
            if (numFacturaBuscada==factura.numeroFactura && factura.anulada==false) {
                encontradoFactura == true;
            }
        }
        if (encontradoFactura==false) {
            cout << "No se ha encontrado la factura\n";
            return;
        }
    } else {
        cout << "Error al abrir el archivo de facturas\n";
        return;
    }
    archivoProductos.open(nombreArchivoProductos, ios::binary);
    if (archivoProductos.good()) {
        cout << "Código del Producto: ";
        cin >> codigoProductoBuscado;
        while (archivoProductos.read((char*)(&producto), sizeof(DatosProducto)) && encontradoCodigo==false) {
            if (codigoProductoBuscado==producto.codigo) {
                encontradoCodigo = true;
            }
        }
        if (encontradoCodigo==false) {
            cout << "El producto no está disponible\n";
            return;
        }
    } else {
        cout << "Error al abrir el archivo de productos\n";
        return;
    }

    archivoReparacion.open(nombreArchivoReparacion, ios::binary | ios::app);
    if (archivoReparacion.good()) {
        repararacion = DatosReparacion(CIClienteBuscado,codigoProductoBuscado,numFacturaBuscada,nombreArchivoFacturas);
        archivoReparacion.write((char*)(&repararacion), sizeof(Reparaciones));
        archivoReparacion.close();
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
void ReporteMensualReparacionesPantalla (string nombreArchivoBinReparaciones, string nombreArchivoBinClientes, string nombreArchivoBinFacturas, string nombreArchivoBinProductos) {
    ifstream archivoBinRepararaciones; //leer
    ifstream archivoBinClientes;
    ifstream archivoBinFacturas;
    ifstream archivoBinProductos;
    Reparaciones reparacion;
    DatosCliente cliente;
    datosFactura factura;
    DatosProducto producto;
    int mesBuscado;
    int anioBuscado;

    archivoBinRepararaciones.open(nombreArchivoBinReparaciones, ios::binary);
    if (archivoBinRepararaciones.good()) {
        cout << "REPORTE MENSUAL DE REPARACIONES\n";
        cout << "Mes del reporte: ";
        cin >> mesBuscado;
        cout << "Año del reporte: ";
        cin >> anioBuscado;
        cout << "===== REPARACIONES DEL MES DE " << meses[mesBuscado-1] << " DE " << anioBuscado << " =====\n";    
        while(archivoBinRepararaciones.read((char*)(&reparacion), sizeof(Reparaciones))) {
            if (reparacion.fechaReparacion.mes==mesBuscado && reparacion.fechaReparacion.anio==anioBuscado) {
                cout << "--------------------------------------------------\n";
                cout << "Fecha: " << reparacion.fechaReparacion.dia << "/" << reparacion.fechaReparacion.mes << "/" << reparacion.fechaReparacion.anio << endl;
                archivoBinClientes.open(nombreArchivoBinClientes, ios::binary);
                if (archivoBinClientes.good()) {
                    cout << "DATOS DEL CLIENTE\n";
                    cout << "CI: " << reparacion.CI_cliente;
                    while(archivoBinClientes.read((char*)(&cliente), sizeof(DatosCliente))) {
                        if (strcmp(reparacion.CI_cliente,cliente.CI_Cliente)==0) {
                            cout << "Nombres y Apellidos: " << cliente.nombre << " " << cliente.apellido << endl;
                            cout << "Tel. y Correo: " << cliente.telefono << " / " << cliente.correo << endl;
                            // PONER  SI ESTÁ ELIMINADO EL CLIENTE O NO
                        }
                    }
                } else {
                    cout << "No se pudo abrir el archivo de clientes\n";
                    return;
                }
                archivoBinClientes.close();

                archivoBinProductos.open(nombreArchivoBinProductos, ios::binary);
                if (archivoBinProductos.good()) {
                    cout << "DATOS DEL PRODUCTO\n";
                    cout << "Código: "  << reparacion.codigoProducto << endl;
                    while (archivoBinProductos.read((char*)(&producto), sizeof(DatosProducto))) {
                        if (reparacion.codigoProducto==producto.codigo) {
                            cout << "Modelo y categoría: " << producto.modelo << ", " << categoriasProductoVector[producto.categoria] << endl;
                            cout << "Precio de Venta: " << producto.precioVenta << endl;
                            if (producto.eliminado) {
                                cout << "NOTA: A fecha de este reporte, este producto no se encuentra disponible.\n";
                            }
                        } 
                    }
                } else {
                    cout << "No se pudo abrir el archivo de productos\n";
                }
                archivoBinProductos.close();

                archivoBinFacturas.open(nombreArchivoBinFacturas, ios::binary);
                if (archivoBinFacturas.good()) {
                    cout << "DATOS DE LA FACTURA\n";
                    cout << "Número de Factura: "  << reparacion.numeroFactura << endl;
                    while (archivoBinFacturas.read((char*)(&factura), sizeof(datosFactura))) {
                        if (reparacion.numeroFactura == factura.numeroFactura && factura.anulada==false) {
                            cout << "Fecha de emisión: " << factura.fecha_emision_factura.dia << "/" << factura.fecha_emision_factura.mes << "/" << factura.fecha_emision_factura.anio << endl;
                        } 
                    }
                } else {
                    cout << "No se pudo abrir el archivo de facturas\n";
                }
                archivoBinFacturas.close();

                cout << "Precio de la Reparación: " << reparacion.precioReparacion << endl;
                cout << "Motivo: " << reparacion.descripcion << endl;
            }
        }
    } else {
        cout << "Error al abrir el archivo de reparaciones\n";
    }
}

bool estaEsteNumeroEnEsteVector(int num, vector<int> vectorCodigos) {
    for(int i=0; i<vectorCodigos.size(); i++) {
        if (num==vectorCodigos[i]) {
            return true;
        }
    }
    return false; // retornará falso si no se encontrara ese numero en el vector de códigos
}

bool existeElProducto (string nombreArchivoProductos, int codigoProducto) {
    ifstream archivo;
    DatosProducto producto;
    archivo.open(nombreArchivoProductos, ios::binary);
    if (archivo.good()) {
        while(archivo.read((char*)(&producto), sizeof(DatosProducto))) {
            if (producto.codigo == codigoProducto && producto.eliminado==false) {
                return true;
            }
        }
    } else {
        cout << "No se pudo abrir el archivo de productos\n";
    }
    archivo.close();
    return false;
}

int codigoProductoMasVecesReparadoDelMes(string nombreArchivoReparaciones, string nombreArchivoProductos, int mes, int anio) {
    ifstream archivoReparaciones;
    Reparaciones reparacion;
    vector<int> codigosTemporalesdelMes = {-1}; //empieza con -1 para que pueda escanear el código la primera vez
    vector<int> reparacionesTemporalSegunCodigo = {0}; // se inicializa igual con un numero para que puedan estar ambos a la par
    int codigoMasReparado;
    int cantidadDeReparacionesMayor = 0;

    archivoReparaciones.open(nombreArchivoReparaciones, ios::binary);
    if (archivoReparaciones.good()) {
        if (anio==reparacion.fechaReparacion.anio && mes==reparacion.fechaReparacion.mes && estaEsteNumeroEnEsteVector(reparacion.codigoProducto,codigosTemporalesdelMes)==false && existeElProducto(nombreArchivoProductos, reparacion.codigoProducto)) {
            codigosTemporalesdelMes.push_back(reparacion.codigoProducto);
            reparacionesTemporalSegunCodigo.push_back(0); // todas las reparaciones están en 0, pero los vectores necestian ser del mismo tamaño
        }
    } else {
        cout << "No se puedo abrir el archivo de Reparaciones\n";
        return;
    }
    archivoReparaciones.close();

    // se vuelve a abrir el archivo esta vez para ver entre cada codigo cuantas ventas tienen
    for (int i=0; i<codigosTemporalesdelMes.size(); i++) {
        if (i>0) { // no se toma en cuenta el primer miebro porque es -1
            archivoReparaciones.open(nombreArchivoReparaciones, ios::binary);
            while(archivoReparaciones.read((char*)(&reparacion),sizeof(Reparaciones))) {
                if (reparacion.codigoProducto == i) {
                    reparacionesTemporalSegunCodigo[i]++; // se van sumando según i las reparaciones sin importar su orden
                }
            }
            archivoReparaciones.close();
        }
    }

    //Ahora se compara la cantidad de reparaciones por codigo
    for (int i=0; i<reparacionesTemporalSegunCodigo.size(); i++) {
        if (i>0 && cantidadDeReparacionesMayor<reparacionesTemporalSegunCodigo[i]) {
            cantidadDeReparacionesMayor = reparacionesTemporalSegunCodigo[i];
            codigoMasReparado = codigosTemporalesdelMes[i];
        }
    }
    return codigoMasReparado;
}


void reporteProductoMensualMasReparadoPantalla(string nombreArchivoReparaciones, string nombreArchivoProductos) {
    ifstream archivoReparaciones;
    ifstream archivoProductos;
    Reparaciones reparacion;
    DatosProducto producto;
    int anioBuscado;
    int mesBuscado;
    int codigoDelProductoMasVecesReparado;
    int contadorReparaciones = 0;

    "REPORTE MENSUAL DE PRODUCTO MÁS REPARADO\n";
    cout << "Ingrese el año del reporte: ";
    cin >> anioBuscado;
    cout << "Ingrese el mes del reporte: ";
    cin >> mesBuscado;
    codigoDelProductoMasVecesReparado = codigoProductoMasVecesReparadoDelMes(nombreArchivoReparaciones, nombreArchivoProductos, mesBuscado, anioBuscado);
    
    archivoProductos.open(nombreArchivoProductos,ios::binary);
    if (archivoProductos.good()) {
        system("cls");
        cout << "=== REPORTE DEL MES DE " << meses[mesBuscado] << " DEL AÑO " << anioBuscado << " ===\n";
        while (archivoProductos.read((char*)(&producto), sizeof(DatosProducto))) {
            if (producto.codigo == codigoDelProductoMasVecesReparado && producto.eliminado==false) {
                cout << "== PRODUCTO MÁS REPARADO DEL MES ==\n";
                cout << "Código: " << producto.codigo << endl;
                cout << "Modelo: " << producto.modelo << endl;
                cout << "Categoría: " << producto.categoria << endl;
                cout << "Precio: " << producto.precioVenta << endl;
                cout << "Stock: " << producto.stock << endl;
                cout << "=========================\n";
            }
        }
        system("pause");
        archivoProductos.close();
    } else {
        cout << "Error al buscar el producto\n";
        system("pause");
        return;
    }

    archivoReparaciones.open(nombreArchivoReparaciones, ios::binary);
    if (archivoReparaciones.good()) {
        cout << "== REPARACIONES DE ESE MES ==\n";
        cout << "FECHA\tCI\tFACTURA\tPRECIO\n";
        while(archivoReparaciones.read((char*)(&reparacion), sizeof(DatosReparacion))) {
            if (reparacion.fechaReparacion.anio==anioBuscado && reparacion.fechaReparacion.mes==mesBuscado && codigoDelProductoMasVecesReparado==reparacion.codigoProducto) {
                cout << reparacion.fechaReparacion.dia << "/" << reparacion.fechaReparacion.mes << "/" << reparacion.fechaReparacion.anio;
                cout << "\t" << reparacion.CI_cliente << "\t" << reparacion.numeroFactura << "\t" << reparacion.precioReparacion << "\n";
            }
        }
    } else {
        cout << "Error al abrir el archivo de Reparaciones\n";
        return;
    }

}

void menuABM_Reparacion(string nombreArchivoBin, string nombreArchivoFactura, string nombreArchivoCliente, string nombreArchivoProducto) {
    int opcion;
    do {
        cout << "== MENÚ REPARACIONES ==\n";
        cout << "\t1. Agregar registro de reparación\n";
        cout << "\t2. Encontrar registro de reparación\n";
        cout << "\t3. Modificar descripción de reaparación\n";
        cout << "\t4. Reporte Mensual: Reparaciones de un mes específico\n";
        cout << "\t5. Reporte Anual: Reparaciones del prodcuto con más reparaciones del mes\n";
        cout << "\t0. Volver\n";
        cout << "--> ";
        cin >> opcion;
        if (opcion==1) {
            agregarDatosReparacion(nombreArchivoBin,nombreArchivoFactura, nombreArchivoCliente, nombreArchivoProducto);
        } else if (opcion==2) {
            encontrarReparacion(nombreArchivoBin);
        } else if (opcion==3) {
            modificarDatosReparacion(nombreArchivoBin);
        } else if(opcion==4) {
            ReporteMensualReparacionesPantalla(nombreArchivoBin, nombreArchivoCliente, nombreArchivoFactura, nombreArchivoProducto);
        } else if (opcion==5) {
            reporteProductoMensualMasReparadoPantalla(nombreArchivoBin,nombreArchivoProducto);
        } else if (opcion==0) {
            cout << "Volviendo al menú principal...\n";
        }
    } while (opcion!=0);

}