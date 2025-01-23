%%改进D-H模型
clear;
clc;
% 设置MDH参数
%       theta    d           a        alpha     offset
SL1=Link([0      0           0        0           0  ],'modified');
SL2=Link([0      0           0        -pi/2       0  ],'modified');
SL3=Link([0      0       330    0           0  ],'modified');
SL4=Link([0      340       0     -pi/2       0  ],'modified');
SL5=Link([0      0           0        pi/2        0  ],'modified');
SL6=Link([0      0           0        -pi/2       0  ],'modified');
% 设置关节角度范围
SL1.qlim=[-45 45]*pi/180;
SL2.qlim=[-50 90]*pi/180;
SL3.qlim=[-135 135]*pi/180;
SL4.qlim=[-180 180]*pi/180;
SL5.qlim=[-90 90]*pi/180;
SL6.qlim=[-180 180]*pi/180;
% 设置关节偏移量
SL1.offset=0;
SL2.offset=-pi/2;
SL3.offset=-pi/2;
SL4.offset=0;
SL5.offset=0;
SL6.offset=0;
% 设置关节初始角度
SL1.theta=0;
SL2.theta=-pi*5/18;    
SL3.theta=pi*3/4;
SL4.theta=0;
SL5.theta=-pi/2;
SL6.theta=0;

p560=SerialLink([SL1 SL2 SL3 SL4 SL5 SL6],'name','puma560');
p560.teach([SL1.theta SL2.theta SL3.theta SL4.theta SL5.theta SL6.theta]);
disp(p560);

%%
clc;
% 1. 计算正向运动学
T = p560.fkine([0 0 0 0 0 0]);  % 计算末端执行器位姿矩阵

% 2. 生成解析解的符号表达式
syms q1 q2 q3 q4 q5 q6 real
q = [q1 q2 q3 q4 q5 q6];

% 使用循环计算每个连杆的变换矩阵
T_sym = eye(4);
fprintf('逐步计算各关节变换矩阵:\n');
for i = 1:6
    % 获取连杆参数
    theta = q(i) + p560.links(i).offset;  % 添加offset
    d = p560.links(i).d;
    a = p560.links(i).a;
    alpha = p560.links(i).alpha;
    
    % 计算ct和st
    ct = cos(theta); st = sin(theta);
    
    % 对alpha进行判断，设置ca和sa
    if abs(alpha) < 1e-8  % alpha接近0
        ca = 1; sa = 0;
    elseif abs(abs(alpha) - pi/2) < 1e-8  % alpha接近±pi/2
        ca = 0; sa = sign(alpha);
    elseif abs(abs(alpha) - pi) < 1e-8  % alpha接近±pi
        ca = -1; sa = 0;
    else
        ca = cos(alpha); sa = sin(alpha);
    end
    
    % 使用改进DH参数的变换矩阵形式
    A = [ct     -st      0       a;
         st*ca   ct*ca  -sa    -sa*d;
         st*sa   ct*sa   ca     ca*d;
         0       0       0       1];
    
    fprintf('\n第%d个关节的变换矩阵:\n', i);
    disp(vpa(A, 4));
    
    T_sym = T_sym * A;
end

% 显示最终简化结果

fprintf('\n最终变换矩阵:\n');
disp(vpa(T_sym, 4));


fprintf('\n三角简化的最终矩阵:\n');
T_simplified = simplify(T_sym);  % 基本简化
T_simplified = collect(T_simplified, [sin(q1), cos(q1)]);  % 收集相似项
disp(vpa(T_simplified, 4));



pos = T_sym * [0 0 0 1]';
display(pos);

fprintf('\n三角简化的末端位置:\n');
pos_simplified = T_simplified * [0 0 0 1]';
disp(vpa(pos_simplified, 4));
%%
% 验证正运动学计算
% 1. 使用自定义符号解
q_val = [30 30 10 20 60 60]*pi/180;  % 测试角度
T_sym_result = subs(T_sym, {q1,q2,q3,q4,q5,q6}, {q_val(1),q_val(2),q_val(3),q_val(4),q_val(5),q_val(6)});
pos_sym = T_sym_result(1:3,4);  % 只取位置向量

T_simplified_result = subs(T_simplified, {q1,q2,q3,q4,q5,q6}, {q_val(1),q_val(2),q_val(3),q_val(4),q_val(5),q_val(6)});
pos_simplified = T_simplified_result(1:3,4);  % 只取位置向量

% 2. 使用 MATLAB Robotics Toolbox
T_matlab = p560.fkine(q_val);  % 数值解
pos_matlab = T_matlab * [0 0 0]';  % 工具箱方式

% 3. 显示对比结果
fprintf('自定义符号解计算结果:\n');
disp(vpa(pos_sym, 4));

fprintf('\n简化符号解计算结果:\n');
disp(vpa(pos_simplified, 4));

fprintf('\nMATLAB工具箱计算结果:\n');
disp(vpa(pos_matlab, 4));

% 4. 计算误差
error = norm(double(pos_simplified) - double(pos_matlab));
fprintf('\n两种方法的计算误差: %e\n', error);

%%
clc;
% 1. 修改目标位置计算方法
p_desired = [193.264; 111.581; -126.586];  % 期望位置
p_target = [p_desired(3); p_desired(2); p_desired(1)];  % 交换x和z坐标

% 2. 计算多组解
% 1. 计算基础变量
r = sqrt(p_target(1)^2 + p_target(2)^2);
s = p_target(3);
A = 340.0; B = 330.0;
cos_q3 = (r^2 + s^2 - A^2 - B^2)/(2*A*B);

% 2. 计算两组q3解（肘up和down）
q3_up = atan2(sqrt(1-cos_q3^2), cos_q3);     % elbow-up解
q3_down = atan2(-sqrt(1-cos_q3^2), cos_q3);  % elbow-down解

% 3. 分别计算对应的q2
% elbow-up解
beta_up = atan2(s, r);
alpha_up = atan2(A*sin(q3_up), B + A*cos(q3_up));
q2_up = beta_up - alpha_up;

% elbow-down解
beta_down = atan2(s, r);
alpha_down = atan2(A*sin(q3_down), B + A*cos(q3_down));
q2_down = beta_down - alpha_down;

% 4. 计算q1
q1_pos = atan2(p_target(2), p_target(1));        % 正解
q1_neg = atan2(-p_target(2), -p_target(1));      % 负解

% 5. 构建多组解
q_sols = [
    q1_pos, q2_up, q3_up;      % 解1
    q1_pos, q2_down, q3_down;   % 解2
    q1_neg, q2_up, q3_up;      % 解3
    q1_neg, q2_down, q3_down    % 解4
];

% 6. 验证所有解
fprintf('所有可能的解（角度）：\n');
disp(q_sols*180/pi);

% 获取关节限位
qlim1 = p560.links(1).qlim;
qlim2 = p560.links(2).qlim;
qlim3 = p560.links(3).qlim;

% 构建多组解并进行限位检查
valid_sols = [];
for i = 1:size(q_sols,1)
    q1_val = q_sols(i,1);
    q2_val = q_sols(i,2);
    q3_val = q_sols(i,3);
    
    % 检查是否在限位范围内
    if q1_val >= qlim1(1)-0.1 && q1_val <= qlim1(2)+0.1 && ...
       q2_val >= qlim2(1)-0.1 && q2_val <= qlim2(2)+0.1 && ...
       q3_val >= qlim3(1)-0.1 && q3_val <= qlim3(2)+0.1
        valid_sols = [valid_sols; q_sols(i,:)];
        
        % 验证有效解
        T_verify = p560.fkine([q1_val, q2_val, q3_val, 0, 0, 0]);
        pos_verify = T_verify * [0 0 0]';
        T_simplified_result = subs(T_simplified, {q1,q2,q3,q4,q5,q6}, {q1_val, q2_val, q3_val, 0, 0, 0});
        pos_simplified = T_simplified_result(1:3,4);  % 只取位置向量
        disp(vpa(pos_simplified, 4));
        fprintf('\n有效解 #%d（角度）:\n', size(valid_sols,1));
        fprintf('q1=%.2f, q2=%.2f, q3=%.2f\n', q1_val*180/pi, q2_val*180/pi, q3_val*180/pi);
        fprintf('位置: [%.2f, %.2f, %.2f]\n', pos_verify(1), pos_verify(2), pos_verify(3));
    end
end

if isempty(valid_sols)
    fprintf('没有找到满足关节限位的解！\n');
end

%%
% clc;
p_target = [-46.475; 1; 252.320];  % 期望位置
A = 340; B = 330;


% q1
if p_target(1) == 0
    q1_low = pi/2;
    q1_high = -pi/2;
elseif p_target(2) == 0
        q1_low = -pi;
        q1_high = 0;
else
    q1_low = atan(p_target(2)/p_target(1));  % 正解
end

if q1_low < 0
    q1_high = q1_low + pi;
end
if q1_low > 0
    q1_high = q1_low;
    q1_low = q1_low - pi;
end


% q2
if p_target(1) == 0
    tp_c = (p_target(3)*p_target(3)+p_target(2)/sin(q1_low)*p_target(2)/sin(q1_low)-A*A-B*B)/(2*A*B);

else
    tp_c = (p_target(3)*p_target(3)+p_target(1)/cos(q1_low)*p_target(1)/cos(q1_low)-A*A-B*B)/(2*A*B);

end

if(tp_c>1)
    delta = -1;
else
    delta = 1;
end
q3_high = acos(tp_c);
q3_low = -q3_high;



% q3
function [q2_slove] = slove_q2(q1_slove,q3_slove,pos)
    A = 340; B = 330;
    TMP_A = A*cos(q3_slove)+B;
    TMP_B = A*sin(q3_slove);
    if(q1_slove ==  pi/2 || q1_slove == -pi/2)
        tmp_cos = (pos(2)*TMP_B/sin(q1_slove)+pos(3)*TMP_A);
        tmp_sin = (pos(2)*TMP_A/sin(q1_slove)-pos(3)*TMP_B);
    else 
        tmp_cos = pos(1)*TMP_B/cos(q1_slove) + pos(3)*TMP_A;
        tmp_sin = pos(1)*TMP_A/cos(q1_slove) - pos(3)*TMP_B;
    end
    q2_slove = atan2(tmp_sin,tmp_cos);
end
if delta < 0
    fprintf('无解\n'); 
    return;
end
q2_1low_3low = slove_q2(q1_low,q3_low,p_target);
q2_1low_3high = slove_q2(q1_low,q3_high,p_target);
q2_1high_3low = slove_q2(q1_high,q3_low,p_target);
q2_1high_3high = slove_q2(q1_high,q3_high,p_target);

q_sols = [
    q1_low, q2_1low_3low, q3_low;      % 解1
    q1_low, q2_1low_3high, q3_high;   % 解2
    q1_high, q2_1high_3low, q3_low;      % 解3
    q1_high, q2_1high_3high, q3_high    % 解4
];


for i = 1:size(q_sols,1)
    fprintf('\n解 #%d（角度）:\n', i);
    disp(q_sols(i,:)*180/pi);
    fprintf('位置: [%.2f, %.2f, %.2f]\n', p_target(1), p_target(2), p_target(3));
    tmp = p560.fkine([q_sols(i,1), q_sols(i,2), q_sols(i,3), 0, 0, 0]);
    pos = tmp * [0 0 0]';
    fprintf('运动位置: [%.2f, %.2f, %.2f]\n', pos(1), pos(2), pos(3));
end

% 获取关节限位
qlim1 = p560.links(1).qlim;
qlim2 = p560.links(2).qlim;
qlim3 = p560.links(3).qlim;

valid_sols = [];
for i = 1:size(q_sols,1)
    q1_val = q_sols(i,1);
    q2_val = q_sols(i,2);
    q3_val = q_sols(i,3);
    
    % 检查是否在限位范围内
    if q1_val >= qlim1(1)-0.1 && q1_val <= qlim1(2)+0.1 && ...
       q2_val >= qlim2(1)-0.1 && q2_val <= qlim2(2)+0.1 && ...
       q3_val >= qlim3(1)-0.1 && q3_val <= qlim3(2)+0.1
        valid_sols = [valid_sols; q_sols(i,:)];
        
        % 验证有效解
        T_verify = p560.fkine([q1_val, q2_val, q3_val, 0, 0, 0]);
        pos_verify = T_verify * [0 0 0]';
        T_simplified_result = subs(T_simplified, {q1,q2,q3,q4,q5,q6}, {q1_val, q2_val, q3_val, 0, 0, 0});
        pos_simplified = T_simplified_result(1:3,4);  % 只取位置向量
        disp(vpa(pos_simplified, 4));
        fprintf('\n有效解 #%d（角度）:\n', size(valid_sols,1));
        fprintf('q1=%.2f, q2=%.2f, q3=%.2f\n', q1_val*180/pi, q2_val*180/pi, q3_val*180/pi);
        fprintf('位置: [%.2f, %.2f, %.2f]\n', pos_verify(1), pos_verify(2), pos_verify(3));
    end
end