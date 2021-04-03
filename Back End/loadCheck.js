import http from 'k6/http';
import { check, sleep } from 'k6';

export let options = {
  stages: [
    { duration: '15s', target: 1 },
    { duration: '15s', target: 10 },
    { duration: '15s', target: 20 },
    { duration: '15s', target: 30 },
    { duration: '55s', target: 40 },
    { duration: '15s', target: 50 },
    { duration: '15s', target: 100 },
    { duration: '30s', target: 200 },
    { duration: '10s', target: 250 },
    { duration: '20s', target: 20 },
  ],
};

export default function () {
  let res = http.get('http://34.117.160.50/user/familyAccount/test@test.com');
  check(res, { 'status was 200': (r) => r.status <= 200 && r.status < 300});
  sleep(1);
}
