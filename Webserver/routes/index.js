var express = require('express');
var router = express.Router();
const gpsdata = require("../models/gpsdata");
/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'IoT project' });
});

router.post("/addgpsdata",(req,res) =>{
  const {assetnumber, latitude, longitude} = req.body;
  let errors = [];

  if(!assetnumber || !latitude || !longitude){
    errors.push({msg : "Parameters are missing"});
  }
  if(errors.length>0){
    res.json({Message : errors})
  }else{
    const newgpsdata = new gpsdata({
      assetnumber,
      latitude,
      longitude
    });

    newgpsdata
    .save()
    .then(newgpsdata => {
      res.json({ Message: "Data Inserted"});
    })
    .catch(err => console.log(err));
  }
});
module.exports = router;

router.get("/getdata/:assetnumber",(req,res) =>{
  var assetnumber = req.params.assetnumber;
  console.log(assetnumber);

  gpsdata.find({assetnumber: assetnumber}).exec((err, notenumber)=>{
    console.log(notenumber);
    res.json(notenumber);
  });
});
/*
{
  "assetnumber" : "1234324",
  "latitude" : "1231313",
  "longitude" : "123145646"
}

*/