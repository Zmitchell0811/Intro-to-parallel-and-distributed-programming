/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Dataset_Parser;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.URL;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 *
 * @author zachary
 */
public class Parser {

    private Pattern iataCodePattern;
    private Pattern icaoCodePattern;
    private Pattern gpsCoordsPattern;
    private Pattern latPattern;
    private Pattern lonPattern;
    private Matcher iataCodes;
    private Matcher icaoCodes;
    private Matcher gpsMatch;
    private Matcher latMatch;
    private Matcher lonMatch;
    private Matcher numMatch;
    private URL datasetPath;
    private FileReader getFile;
    private PrintWriter out;
    private ArrayList<String> dataLines;
    private ArrayList<String> iataCode;
    private ArrayList<String> icaoCode;
    private ArrayList<String> latCoord;
    private ArrayList<String> lonCoord;
    private ArrayList<String> gpsCoord;
    private ArrayList<Double> latRadians;
    private ArrayList<Double> lonRadians;
    private ArrayList<Double> distances;
    private Double tempDist;

    private String filePath;

    //Why are we here?
    //To make the world a better place of course!
    
    public Parser() {
        iataCodePattern = Pattern.compile("([\\p{Upper}]{3})");
        icaoCodePattern = Pattern.compile("([\\p{Upper}]{4})");
        gpsCoordsPattern = Pattern.compile("([-+]?)([\\d]{1,2})(((\\.)(\\d+)(,)))(\\s*)(([-+]?)([\\d]{1,3})((\\.)(\\d+))?)"); //Regular expression found at https://www.regextester.com/93451
        latPattern = Pattern.compile("^([-+]?)([\\d]{1,2})(((\\.)(\\d+)))");
        lonPattern = Pattern.compile("(([-+]?)([\\d]{1,3})((\\.)(\\d+))?)$");
        filePath = "/home/zachary/NetBeansProjects/Para_Dist_Project/src/resources/data.dat";
        dataLines = new ArrayList<>();
        iataCode = new ArrayList<>();
        icaoCode = new ArrayList<>();
        gpsCoord = new ArrayList<>();
        latCoord = new ArrayList<>();
        lonCoord = new ArrayList<>();
        latRadians = new ArrayList<>();
        lonRadians = new ArrayList<>();
        distances = new ArrayList<>();
    }

    public void beautifyData() {
        try {
            //Initialize step
            BufferedReader in;
            getFile = new FileReader(filePath);
            in = new BufferedReader(getFile);

            String fileLines;
            try {
                fileLines = in.readLine();

                while (fileLines != null) {
                    dataLines.add(fileLines);
                    fileLines = in.readLine();
                }
                dataLines.remove(1963);//Some stupid error
                dataLines.remove(4671);//Another stupid error

                for (int i = 0; i < dataLines.size(); i++) {
                    iataCodes = iataCodePattern.matcher(dataLines.get(i));
                    icaoCodes = icaoCodePattern.matcher(dataLines.get(i));
                    gpsMatch = gpsCoordsPattern.matcher(dataLines.get(i));
                    try {
                        if ((iataCodes.find() != false) && (icaoCodes.find() != false) && (gpsMatch.find() != false)) {
                            
                            iataCode.add(iataCodes.group());
                            
                            icaoCode.add(icaoCodes.group());
                            
                            gpsCoord.add(gpsMatch.group());
                        }
                        
                    } catch (Exception ex) {
                        System.out.println("Error:" + ex);

                    }

                }
                for (int i = 0; i < gpsCoord.size(); i++) {
                    latMatch = latPattern.matcher(gpsCoord.get(i));
                    lonMatch = lonPattern.matcher(gpsCoord.get(i));
                    latMatch.find();
                    lonMatch.find();
                    latCoord.add(latMatch.group());
                    lonCoord.add(lonMatch.group());
                }
                for (int i = 0; i < latCoord.size(); i++) {
                    latRadians.add(Double.parseDouble(latCoord.get(i))* Math.PI/180);
                    lonRadians.add(Double.parseDouble(lonCoord.get(i))* Math.PI/180);
                }
                for (int i = 0; i < latRadians.size(); i++) {
                    System.out.println(latRadians.get(i));
                    System.out.println(lonRadians.get(i));
                    System.out.println(iataCode.get(i));
                    System.out.println(icaoCode.get(i));
                }
                
                
            } catch (IOException ex) {
                System.out.println("Error: " + ex);
            }
        } catch (FileNotFoundException e) {
            System.out.println("Error: File not found");
        }
    }
}
