from requests import Session

session = Session()

# HEAD requests ask for *just* the headers, which is all you need to grab the
# session cookie
session.head('https://catalog.paytm.com')

response = session.post(
    url='https://catalog.paytm.com/v1/g/recharge-plans/mobile/2g-data?operator=BSNL&circle=Karnataka&type=mobile&description=1&page_count=1&items_per_page=30&sort_price=0&callback=angular.callbacks._9',
    data={
        'N': '4294966750',
        'form-trigger': 'moreId',
        'moreId': '156#327',
        'pageType': 'EventClass'
    },
    headers={
        'Referer': 'http://sportsbeta.ladbrokes.com/football'
    }
)

print response.text

GET  HTTP/1.1
Host: catalog.paytm.com
Connection: keep-alive
Accept: */*
User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.90 Safari/537.36
Referer: https://paytm.com/
Accept-Encoding: gzip, deflate, sdch
Accept-Language: en-US,en;q=0.8
Cookie: __utma=101648542.33676893.1426944592.1426944622.1426944622.1; __utmc=101648542; __utmz=101648542.1426944622.1.1.utmcsr=paytm.com|utmccn=(referral)|utmcmd=referral|utmcct=/coupons; vizEid=272b4f3f6fc5af1e50edf3c5c78935ae; _ga=GA1.2.33676893.1426944592; _gat=1

