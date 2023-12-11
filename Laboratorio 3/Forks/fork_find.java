
import java.io.File;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.concurrent.RecursiveTask;
import java.util.concurrent.ForkJoinPool;

public class fork_find {
    public static class Sopa {
        public int tamanio;
        public String abuscar;
        public String[][] letras;

        public void settamanio(int dato) {
            this.tamanio = dato;
            this.letras = new String[dato][dato];
        }

        public void setpalabra(String dato) {
            this.abuscar = dato;
        }
    }

    public static class Busqueda extends RecursiveTask<String> {
        private String[][] letras;
        private String palabra;
        private int inicioX, inicioY;
        private int finX;
        private int finY;
        private int tamanio;
    
        public Busqueda(String[][] letras, String palabra, int inicioX, int inicioY, int finX, int finY, int tamanio) {
            this.letras = letras;
            this.palabra = palabra;
            this.inicioX = inicioX;
            this.inicioY = inicioY;
            this.finX = finX;
            this.finY = finY;
            this.tamanio = tamanio;
        }
    
        
        protected String compute() {
            if(tamanio == palabra.length()){ 
                for (int x = inicioX; x <= finX; x++) {
                    for (int y = inicioY; y < finY; y++) {
                        if (buscarHorizontal(x, y)) {
                            return "encontrada horizontalmente: Columna: "+(y+1) + " y Fila: " + (x+1);
                        }
                    }
                }
                for (int x = inicioX; x < finX; x++) {
                    for (int y = inicioY; y <=finY; y++) {
                        if (buscarVertical(x, y)) {
                            return "encontrada verticalmente: Columna: "+(y+1) + " y Fila: " + (x+1);
                        }
                    }
                }
            }else{
                int new_tamanio = tamanio/2;
                Busqueda[] cuadrantes = new Busqueda[3];
                cuadrantes[0] = new Busqueda(letras, palabra,(finX+inicioX)/2 ,inicioY,finX,(finY+inicioY)/2-1,new_tamanio);
                cuadrantes[1] = new Busqueda(letras, palabra, inicioX, (finY+inicioY) /2, (finX+inicioX)/2 -1, finY,new_tamanio);
                cuadrantes[2] = new Busqueda(letras, palabra, (finX+inicioX)/2 , (finY+inicioY)/2, finX, finY, new_tamanio);

                invokeAll(cuadrantes);
                finX = (finX+inicioX)/2 -1;
                finY = (finY+inicioY)/2 -1;
                tamanio = new_tamanio;
                String result = compute();
                if(result == null){
                result =  cuadrantes[0].join();
                }
                if(result == null){
                result =  cuadrantes[1].join();
                }
                if(result == null){
                result =  cuadrantes[2].join();
                }
                return result;
            }
            return null; // Reemplaza con el resultado real
        } 
        private boolean buscarHorizontal(int x, int y) {
            for (int i = 0; i < palabra.length(); i++) {
                if (letras[x][y + i].charAt(0) != palabra.charAt(i)) {
                    return false;
                }
            }
            return true;
        }
    
        private boolean buscarVertical(int x, int y) {
            
            for (int i = 0; i < palabra.length(); i++) {
                if (letras[x + i][y].charAt(0) != palabra.charAt(i)) {
                    return false;
                }
            }
            return true;
        }
    }
    

    public static void main(String[] args) {
        System.out.println("----------------------------------------------------------------------------");
        System.out.println("Busqueda Mediante Forks:");
        Integer total = 0;
        Integer contador = 0;
        File directorio = new File("Laberintos");
        File[] carpetas = directorio.listFiles();
        if (carpetas != null) {
            for (File carpeta : carpetas) {
                if (carpeta.isDirectory()) {
                    Sopa soup = new Sopa();
                    leerArchivo(carpeta, soup);
                    String palabra = soup.abuscar;
                    ForkJoinPool pool = new ForkJoinPool();
                    Busqueda task = new Busqueda(soup.letras, palabra, 0, 0, soup.letras.length - 1, soup.letras[0].length - 1,soup.tamanio);
                    System.out.println("----------------------------------------------------------------------------");
                    System.out.println("Buscando: "+ soup.abuscar);
                    long tiempoInicial = System.currentTimeMillis();
                    String result = pool.invoke(task);
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
