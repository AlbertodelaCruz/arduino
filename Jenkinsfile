pipeline {
  agent any
  stages {
    stage('image') {
      steps {
        sh 'ls'
      }
    }
    stage('sleep') {
      steps {
        sleep 10
      }
    }
    stage('alepre') {
      steps {
        waitUntil()
      }
    }
  }
}