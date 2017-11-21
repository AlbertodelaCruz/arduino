//This lives in your project repo in a file called Jenkinsfile
try {
  stage("Build") {
    //Assuming your build machines are labeled 'linux'
    node('linux') {
      //Checkout the project as specified in the git config
      checkout scm
      //do build stuff
      sh "put your build command here"
      //save files for later
      stash includes: '**', name: 'build'
    }

    //Requires email plugin of course
    mail(to: 'EMAIL@example.com',
         subject: "${currentBuild.fullDisplayName} is ready for deployment",
         body: "URL: ${env.BUILD_URL}")
  }

  //Not required, but sensible - this will automatically abort the build if you wait too long
  timeout(time: 1, unit: 'WEEK') {
    input "Approve/deny deployment to production system"
  }

  //Optional - prevents older waiting builds from deploying if a newer build was already approved and got past this
  milestone label: "production"

  stage("Production Deploy") {
    node('linux') {
      //restore previously saved files
      unstash 'build'
      //do deploy stuff
      sh("deploy shell command might go here")
    }
  }
} catch(Error|Exception e) {
  mail(to: 'EMAIL@example.com',
       subject: "${currentBuild.fullDisplayName} failed!",
       body: "URL: ${env.BUILD_URL}, Error: ${e}")
  //Finish failing the build after telling someone about it
  throw e
}
