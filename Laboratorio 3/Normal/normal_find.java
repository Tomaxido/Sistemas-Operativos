
import java.io.File;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;


public class normal_find {
    public static class Sopa {
        public int tamanio;
        public String abuscar;
        public String[][] letras;
        public int filaEncontrada = -1;
        public int columnaEncontrada = -1;

        public void settamanio(int dato) {
            this.tamanio = dato;
            this.letras = new String[dato][dato];
        }

        public void setpalabra(String dato) {
            this.abuscar = dato;
        }

        public String busqueda() {
            // Buscar horizontalmente
            for (int x = 0; x < tamanio; x++) {
                for (int y = 0; y <= tamanio - abuscar.length(); y++) {
                    boolean encontrado = true;
                    for (int i = 0; i < abuscar.length(); i++) {
                        if (!letras[x][y + i].equalsIgnoreCase(String.valueOf(abuscar.charAt(i)))) {
                            encontrado = false;
                            break;
                        }
                    }
                    if (encontrado) {
                        filaEncontrada = x;
                        columnaEncontrada = y;
                        return "Encontrada Horizontalmente: Columna: " + (columnaEncontrada+1) + " y Fila " + (filaEncontrada+1);
                    }
                }
            }
    
            // Buscar verticalmente
            for (int x = 0; x <= tamanio - abuscar.length(); x++) {
                for (int y = 0; y < tamanio; y++) {
                    boolean encontrado = true;
                    for (int i = 0; i < abuscar.length(); i++) {
                        if (!letras[x + i][y].equalsIgnoreCase(String.valueOf(abuscar.charAt(i)))) {
                            encontrado = false;
                            break;
                        }
                    }
                    if (encontrado) {
                        filaEncontrada = x;
                        columnaEncontrada = y;
                        return "Encontrada verticalmente: Columna: " + (columnaEncontrada+1) + " y Fila " + (filaEncontrada+1);
                    }
                }
            }
    
            return "No encontrada";
        }
    
    }


    public static void main(String[] args) {
        System.out.println("----------------------------------------------------------------------------");
        System.out.println("Busqueda Normal:");
        Integer total = 0;
        Integer contador = 0;
        File directorio = new File("Laberintos");
        File[] carpetas = directorio.listFiles();
        if (carpetas != null) {
            for (File carpeta : carpetas) {
                if (carpeta.isDirectory()) {
                    Sopa soup = new Sopa();
                    leerArchivo(carpeta, soup);
                    System.out.println("----------------------------------------------------------------------------");
                    System.out.println("Buscando: "+ soup.abuscar);
                    long tiempoInicial = System.currentTimeMillis();
                    String result = soup.busqueda();
                    long tiempoFinal = System.currentTimeMillis();
                    long tiempoTotal = tiempoFinal - tiempoInicial;
                    total += (int) tiempoTotal;
                    contador++;
                    if (result != null) {
                        System.out.println(" -Palabra " + result);
                        System.out.println(" -Tiempo de ejecución: " + tiempoTotal + " milisegundos");
                    } else {
                        System.out.println("Palabra no encontrada.");
                    }  
                }
            }
            System.out.println("----------------------------------------------------------------------------");
            System.out.println(" -Tiempo promedio de ejecución: " + total/contador + " milisegundos");
            System.out.println("----------------------------------------------------------------------------");
        }
    }

    public static void leerArchivo(File carpeta, Sopa lab) {
        File[] archivos = carpeta.listFiles();
        if (archivos != null) {
            for (File archivo : archivos) {
                try (BufferedReader br = new BufferedReader(new FileReader(archivo))) {
                    String linea;
                    int contador = 0;
                    while ((linea = br.readLine()) != null) {
                        if (contador == 0) {
                            lab.settamanio(Integer.parseInt(linea));
                        } else if (contador == 1) {
                            lab.setpalabra(linea);
                        } else {
                            lab.letras[contador - 2] = linea.split(" ");
                        }
                        contador++;
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
