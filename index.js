const os = require("os");
const { execSync } = require("child_process");

const isWindows = os.platform() === "win32";
const projectRoot = __dirname;

function runCommand(cmd) {
  try {
    execSync(cmd, { stdio: "inherit", cwd: projectRoot });
  } catch (err) {
    console.error(`Command failed: ${cmd}`);
    process.exit(1);
  }
}

function run() {
  console.log("\n💻 System Information:");
  console.log("----------------------");
  // System Information
  console.log("OS Platform: ", os.platform());
  console.log("OS Release: ", os.release());
  console.log("OS Type: ", os.type());
  console.log("Hostname: ", os.hostname());
  console.log("Architecture: ", os.arch());
  // CPU Information
  const cpus = os.cpus();
  console.log("CPU Cores: ", cpus.length);
  console.log("CPU Model: ", cpus[0].model);
  console.log("CPU Speed (MHz): ", cpus[0].speed);
  // Memory Information
  console.log("Memory: ", `${(os.totalmem() / 1024 / 1024).toFixed(2)} MB`);
  console.log("----------------------\n");
  const script = isWindows ? "./scripts/run.ps1" : "bash ./scripts/run.sh";
  runCommand(script);
}

run();
