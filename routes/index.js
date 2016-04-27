var express = require('express');
var spawn = require('child_process').spawn;
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index',
    {
      title: 'CS Faculty / University Searcher',
      readme: 'Input a university name or faculty name, and magic happens!',
      queryResult: '',
      lastKeyword: ''
      //alertContent: 'This is an alert'
    });
});

/* Search Request */
router.post('/', function(req, res) {

  keyword = req.body.keyword;
  console.log("body.keyword: " + keyword);
  queryCgi = spawn('cgi-bin/query', [keyword]);

  queryResult = "";
  queryCgi.stdout.on('data', function (data) {
      queryResult += data;
  });

  queryCgi.stdout.on('end', function(){
      console.log('standard output:\n' + queryResult);
      res.render('index',
        {
          title: 'CS Faculty / University Searcher',
          keyword: keyword,
          alertContent: 'Last search: "<strong>' + keyword + '"</strong> ' + new Date(),
          queryResult: JSON.parse(queryResult)
        });
  })
});

router.get('/about', function(req, res, next) {
  res.render('about',
    {
      title: 'CS Faculty / University Searcher',
      content: ["This is our demo for Computer Network course project.", "April 2016"]
    });
});

router.get('/contact', function(req, res, next) {
  res.render('contact',
    {
      title: 'CS Faculty / University Searcher',
      content: 'Below is our contact information:',
      infos: {
          'Shuyang Shi': 'shuyang790@gmail.com',
          'Xingyu Lin': 'sean.linxingyu@pku.edu.cn',
          'Dayou Du': 'dudayou1901@gmail.com',
          'Yize Xie': 'xyzoffice@163.com'
      }
    });
});

module.exports = router;
