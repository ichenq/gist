    
public class Utils {

    public static String GetCPUSerial() {
        String serial = "";
        try {
            File file = new File("/proc/cpuinfo");
            if (file.exists()) {
                BufferedReader reader = new BufferedReader(new FileReader(file));
                while (true) {
                    String line = reader.readLine();
                    if (line == null) {
                        break;
                    }
                    int index = line.indexOf("Serial");
                    if (index >= 0) {
                        serial = line.substring(index + 1).trim();
                        Log.i("cpu serial", serial);
                        break;
                    }
                }
            }
        } catch (Exception ex) {
            Log.e("BeyondEngine", ex.toString());
        }
        return serial;
    }
    
    public static String GetOrCreateFileUUID() {
        String uniqueString = "";
        String filepath = GetExternalDirectoryPath() + "/guid";
        try {
            File file = new File(filepath);
            if (!file.exists()) {
                if (file.createNewFile()) {
                    uniqueString = UUID.randomUUID().toString();
                    OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(file));
                    writer.write(uniqueString);
                    writer.close();
                } else {
                    Log.e("BeyondEngine", "cannot create uuid file " + filepath);
                }
            } else {
                BufferedReader reader = new BufferedReader(new FileReader(file));
                String line = reader.readLine();
                if (line != null) {
                    uniqueString = line.trim();
                }
            }
        } catch (Exception ex) {
            Log.e("BeyondEngine", String.format("cannot create uuid file %s, %s", filepath, ex.toString()));
        }
        return uniqueString;
    }
    
    // Get an unique ID of this device
    // https://developer.android.com/training/articles/user-data-ids#java
    public static String GetUniqueDeviceID() {
        String uniqueText = GetOrCreateFileUUID();
        if (uniqueText == "") {
            String serial = android.os.Build.SERIAL;
            if (serial != null) {
                uniqueText += serial;
            }
            String androidID = Settings.Secure.getString(_activity.getContentResolver(), Settings.Secure.ANDROID_ID);
            if (androidID != null) {
                uniqueText += androidID;
            }
            String cpuSerial = GetCPUSerial();
            if (cpuSerial != "") {
                uniqueText += cpuSerial;
            }
        }
        return uniqueText;
    }    
}    